#include "XUImpl.h"
#include "XUStorage.h"
#include "TUDeviceInfo.h"
#include "TUJsonHelper.h"
#include "XULanguageManager.h"
#include "TapUELogin.h"
#include "TUHelper.h"
#include "TUHUD.h"
#include "URLParser.h"
#include "XDGSDK.h"
#include "XDUE.h"
#include "XUConfigManager.h"
#include "XUThirdAuthHelper.h"
#include "XUThirdPayHelper.h"
#include "XDGSDK/UI/XUAccountCancellationWidget.h"
#include "XDGSDK/UI/XUPayWebWidget.h"
#include "XDGSDK/UI/XUPrivacyWidget.h"
#include "Track/XUTracker.h"
#include "Track/XUPaymentTracker.h"
#include "Agreement/XUAgreementManager.h"

static int Success = 200;

XUImpl::FSimpleDelegate XUImpl::OnLoginSuccess;
XUImpl::FSimpleDelegate XUImpl::OnLogoutSuccess;
XUImpl::FSimpleDelegate XUImpl::OnTokenIsInvalid;

void XUImpl::InitSDK(XUInitCallback CallBack, TFunction<void(TSharedRef<XUType::Config> Config)> EditConfig) {
	if (InitState == Initing) {
		if (CallBack) {
			CallBack(false, "XD SDK Initing");
		}
		return;
	}
	if (InitState == Inited) {
		if (CallBack) {
			CallBack(true, "XD SDK has Inited");
		}
		return;
	}
	InitState = Initing;
	XUConfigManager::ReadLocalConfig([=](TSharedPtr<XUType::Config> Config, const FString& Msg) {
		InitState = Uninit;
		if (Config.IsValid()) {
			if (EditConfig) {
				EditConfig(Config.ToSharedRef());
			}
			InitSDK(Config, CallBack);
		} else {
			if (CallBack) {
				CallBack(false, Msg);
			}
		}
	});
}

void XUImpl::InitSDK(TSharedPtr<XUType::Config> Config, XUInitCallback CallBack) {
	if (InitState == Initing) {
		if (CallBack) {
			CallBack(false, "XD SDK Initing");
		}
		return;
	}
	if (InitState == Inited) {
		if (CallBack) {
			CallBack(true, "XD SDK has Inited");
		}
		return;
	}
	InitState = Initing;
	XUInitCallback NewCallBack = [=](bool Result, const FString& Message) {
		if (Result) {
			InitState = Inited;
		} else {
			InitState = Uninit;
		}
		if (CallBack) {
			CallBack(Result, Message);
		}
	};
	// 用本地缓存更新当前 config
	XUConfigManager::UpdateConfigWithCache();

	// 海外请求最新协议内容，国内直接返回
	XUAgreementManager::RequestServerAgreementsExceptCN([=](bool ResultSuccess) {
		// 检查协议
		XUAgreementManager::CheckAgreementWithHandler([=]() {
			// 协议完成
			InitFinish(NewCallBack);
		});
	});
}

void XUImpl::LoginByType(XUType::LoginType LoginType,
                         TFunction<void(TSharedPtr<FXUUser> user)> resultBlock,
                         TFunction<void(FXUError error)> ErrorBlock) {
	auto lmd = XULanguageManager::GetCurrentModel();
	if (LoginType == XUType::Default) {
		bool TokenInfoIsInvalid = TUDataStorage<FXUStorage>::LoadBool(FXUStorage::TokenInfoIsInvalid);
		if (TokenInfoIsInvalid) { // 如果token已经失效了, 清除登录缓存及协议
			FXUUser::ClearUserData();
			OnTokenIsInvalid.Broadcast();
			if (ErrorBlock) {
				ErrorBlock(FXUError(lmd->tds_login_failed));
			}
			return;
		}
		auto localUser = XDUE::GetUserInfo();
		if (localUser.IsValid()) {
			RequestUserInfo(true, [](TSharedPtr<FXUUser> user) {}, [](FXUError Error) {});
			AsyncLocalTdsUser(localUser->userId, FXUSyncTokenModel::GetLocalModel()->sessionToken);
			LoginSuccess(localUser, resultBlock);
		}
		else {
			if (ErrorBlock) {
				ErrorBlock(FXUError(lmd->tds_login_failed));
			}
		}
	}
	else {
		// 如果已经是登录状态了，那么不再重复登录
		if (XDUE::GetUserInfo().IsValid()) {
			if (ErrorBlock) {
				ErrorBlock(FXUError("The user is logged in"));
			}
			return;
		}
		GetAuthParam(LoginType, [=](TSharedPtr<FJsonObject> paras) {
			if (FXUTokenModel::GetLocalModel().IsValid()) {
				TUDebuger::WarningLog("Login Token Has Exist");
				return;
			}
			UTUHUD::ShowWait();
			TFunction<void(FXUError error)> ErrorCallBack = [=](FXUError error) {
				UTUHUD::Dismiss();
				FXUUser::ClearUserData();
				if (ErrorBlock) {
					ErrorBlock(error);
				}
			};
			RequestKidToken(paras, [=](TSharedPtr<FXUTokenModel> kidToken) {
				RequestUserInfo(false, [=](TSharedPtr<FXUUser> user) {
					AsyncNetworkTdsUser(user->userId, [=](FString SessionToken) {
						UTUHUD::Dismiss();
						user->SaveToLocal();
						LoginSuccess(user, resultBlock);
					}, ErrorCallBack);
				}, ErrorCallBack);
			}, ErrorCallBack);
		}, ErrorBlock);
	}
}

void XUImpl::GetAuthParam(XUType::LoginType LoginType,
                                 TFunction<void(TSharedPtr<FJsonObject> paras)> resultBlock,
                                 TFunction<void(FXUError error)> ErrorBlock) {
	if (LoginType == XUType::Guest) {
		XUThirdAuthHelper::CancelAllPreAuths();
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		JsonObject->SetNumberField("type", (int)LoginType);
		JsonObject->SetStringField("token", TUDeviceInfo::GetLoginId());
		resultBlock(JsonObject);
	}
	else if (LoginType == XUType::TapTap) {
		XUThirdAuthHelper::TapTapAuth(
			[=](FTUAccessToken AccessToken) {
				TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
				JsonObject->SetNumberField("type", (int)LoginType);
				JsonObject->SetStringField("token", AccessToken.kid);
				JsonObject->SetStringField("secret", AccessToken.mac_key);
				resultBlock(JsonObject);
			}, ErrorBlock);
	}
	else if (LoginType == XUType::Google) {
		TUDebuger::DisplayLog("Google Login");
		XUThirdAuthHelper::WebAuth(XUThirdAuthHelper::GoogleAuth,
			[=](TSharedPtr<FJsonObject> AuthParas) {
				AuthParas->SetNumberField("type", (int)LoginType);
				resultBlock(AuthParas);
			}, ErrorBlock);
	}
	else if (LoginType == XUType::Apple) {
		TUDebuger::DisplayLog("Apple Login");
		XUThirdAuthHelper::WebAuth(XUThirdAuthHelper::AppleAuth,
			[=](TSharedPtr<FJsonObject> AuthParas) {
				AuthParas->SetNumberField("type", (int)LoginType);
				resultBlock(AuthParas);
			}, ErrorBlock);
	}
	else {
		ErrorBlock(FXUError("No Login Param"));
	}
}

void XUImpl::CheckPay(TFunction<void(XUType::CheckPayType CheckType)> SuccessBlock,
                            TFunction<void(const FXUError& Error)> FailBlock) {
	XUNet::CheckPay([=](TSharedPtr<FXUPayCheckResponseModel> Model, FXUError Error) {
		if (Model.IsValid()) {
			bool hasIOS = false;
			bool hasAndroid = false;
			for (auto md : Model->list) {
				if (md.platform == 1) {
					hasIOS = true;
				}
				if (md.platform == 2) {
					hasAndroid = true;
				}
			}
			if (SuccessBlock) {
				if (hasIOS && hasAndroid) {
					SuccessBlock(XUType::iOSAndAndroid);
				}
				else if (hasIOS) {
					SuccessBlock(XUType::iOS);
				}
				else if (hasAndroid) {
					SuccessBlock(XUType::Android);
				}
				else {
					SuccessBlock(XUType::None);
				}
			}
		}
		else {
			if (FailBlock) {
				FailBlock(Error);
			}
		}
	});
}

FString XUImpl::GetCustomerCenter(const FString& ServerId, const FString& RoleId, const FString& RoleName) {
	auto userMd = FXUUser::GetLocalModel();
	auto cfgMd = XUConfigManager::CurrentConfig();
	auto tkModel = FXUTokenModel::GetLocalModel();
	if (!userMd.IsValid() || !cfgMd.IsValid() || !tkModel.IsValid()) {
		return "";
	}
	
	TSharedPtr<FJsonObject> query = MakeShareable(new FJsonObject);
	query->SetStringField("client_id", XUConfigManager::CurrentConfig()->ClientId);
	query->SetStringField("access_token", tkModel->kid);
	query->SetStringField("user_id", userMd->userId);
	query->SetStringField("server_id", ServerId);
	query->SetStringField("role_id", RoleId);
	query->SetStringField("role_name", RoleName);
	query->SetStringField("region", cfgMd->Region);
	query->SetStringField("sdk_ver", XDUESDK_VERSION);
	query->SetStringField("sdk_lang", XULanguageManager::GetCustomerCenterLang());
	query->SetStringField("app_ver", XUConfigManager::CurrentConfig()->GameVersion);
	query->SetStringField("app_ver_code", XUConfigManager::CurrentConfig()->GameVersion);
	query->SetStringField("res", FString::Printf(TEXT("%d_%d"), TUDeviceInfo::GetScreenWidth(), TUDeviceInfo::GetScreenHeight()));
	query->SetStringField("cpu", TUDeviceInfo::GetCPU());
	query->SetStringField("pt", TUDeviceInfo::GetPlatform());
	query->SetStringField("os", TUDeviceInfo::GetOSVersion());
	query->SetStringField("brand", TUDeviceInfo::GetGPU());
	query->SetStringField("game_name", TUDeviceInfo::GetProjectName());

	FString QueryStr = TUHelper::CombinParameters(query);
	FString UrlStr = cfgMd->ReportUrl;
	auto Parse = TUCommon::FURL_RFC3986();
	Parse.Parse(UrlStr);
	UrlStr = FString::Printf(TEXT("%s://%s"), *Parse.GetScheme(), *Parse.GetHost()) / Parse.GetPath();
	UrlStr = UrlStr + "?" + QueryStr;
	return UrlStr;
}

void XUImpl::OpenWebPay(const FString& ServerId, const FString& RoleId, const FString& ProductId,
	const FString& OrderId, const FString& ProductName, float PayAmount, const FString& Ext,
	TFunction<void(XUType::PayResult Result)> CallBack) {
	TSharedPtr<FJsonObject> Query = MakeShareable(new FJsonObject);
	if (ProductId.IsEmpty()) {
		TUDebuger::ErrorLog("ProductId is empty");
		return;
	}
	XUPaymentTracker::PaymentStart(ProductId);
	if (RoleId.IsEmpty()) {
		TUDebuger::ErrorLog("RoleId is empty");
		return;
	}
	if (ServerId.IsEmpty()) {
		TUDebuger::ErrorLog("ServerId is empty");
		return;
	}
	Query->SetStringField("productSkuCode", ProductId);
	Query->SetStringField("roleId", RoleId);
	Query->SetStringField("serverId", ServerId);

	if (!OrderId.IsEmpty()) {
		Query->SetStringField("orderId", OrderId);
	}

	if (!ProductName.IsEmpty()) {
		Query->SetStringField("productName", ProductName);
	}
	if (PayAmount > 0) {
		Query->SetStringField("payAmount", FString::Printf(TEXT("%f"), PayAmount));
	}
	if (!Ext.IsEmpty()) {
		Query->SetStringField("extras", Ext);
	}
	Query->SetStringField("region", XUConfigManager::CurrentConfig()->Region);
	Query->SetStringField("appId", XUConfigManager::CurrentConfig()->AppID);
	Query->SetStringField("lang", XULanguageManager::GetLanguageKey());
	Query->SetStringField("platform", TUDeviceInfo::GetPlatform());
	Query->SetStringField("eventSessionId", XUPaymentTracker::GetCurrentEventSessionId());
	
	int64 TimeStamp = FDateTime::UtcNow().ToUnixTimestamp();
	FString XDClientId = XUConfigManager::CurrentConfig()->ClientId;
	FString SignStr = FString::Printf(TEXT("%s%s%s%lld%s"), *ProductId, *RoleId, *ServerId, TimeStamp, *XDClientId);
	FString SignMD5 = FString::Printf(TEXT("%s,%lld"), *FMD5::HashAnsiString(*SignStr), TimeStamp); 
	
	FString QueryStr = TUHelper::CombinParameters(Query);
	FString UrlStr = XUConfigManager::CurrentConfig()->WebPayUrl + "?" + QueryStr + "&sign=" + SignMD5;


	auto NewCallBack = [=](XUType::PayResult Result) {
		if (Result == XUType::PayOK) {
			XUPaymentTracker::PaymentDone();
		} else if (Result == XUType::PayCancel) {
			XUPaymentTracker::PaymentFailed("user_cancel");
		} else {
			XUPaymentTracker::PaymentFailed("fail");
		}
		
		if (CallBack) {
			CallBack(Result);
		}
	};
	if (XUConfigManager::IsCN()) {
		UXUPayWebWidget::Show(UrlStr, NewCallBack);
	} else {
		XUThirdPayHelper::StartWebPay(UrlStr, NewCallBack);
	}
	XUPaymentTracker::CallPaymentPage();
}


void XUImpl::ResetPrivacy() {
	TUDataStorage<FXUStorage>::Remove(FXUStorage::PrivacyKey);
	XUAgreementManager::ResetAgreement();
}

void XUImpl::AccountCancellation() {
	TSharedPtr<FJsonObject> Query = MakeShareable(new FJsonObject);
	auto AccessToken = FXUTokenModel::GetLocalModel();
	Query->SetStringField("kid", AccessToken->kid);
	Query->SetStringField("mac_key", AccessToken->macKey);
	Query->SetStringField("source", "game");
	Query->SetStringField("cn", XUConfigManager::IsCN() ? "1" : "0");
	Query->SetStringField("sdk_lang", XULanguageManager::GetLanguageKey());
	Query->SetStringField("version", XDUESDK_VERSION);
	Query->SetStringField("client_id", XUConfigManager::CurrentConfig()->ClientId);

	auto Local = FXUIpInfoModel::GetLocalModel();
	if (Local.IsValid()) {
		Query->SetStringField("country_code", Local->country_code);
	}
	Query->SetStringField("time", FString::Printf(TEXT("%lld"), FDateTime::UtcNow().ToUnixTimestamp()));

	FString QueryStr = TUHelper::CombinParameters(Query);
	FString UrlStr = XUConfigManager::CurrentConfig()->LogoutUrl + "?" + QueryStr;

	UXUAccountCancellationWidget::Show(UrlStr);
}

void XUImpl::Logout() {
	// await TDSUser.Logout();
	TapUELogin::Logout();
	FXUUser::ClearUserData();
	OnLogoutSuccess.Broadcast();
}

TSharedPtr<XUImpl> XUImpl::Instance = nullptr;

TSharedPtr<XUImpl>& XUImpl::Get() {
	if (!Instance.IsValid()) {
		Instance = MakeShareable(new XUImpl);
		XDUE::OnUserStatusChange.AddLambda([](XUType::UserChangeState UserState, const FString& Msg) {
			if (UserState == XUType::UserLogout) {
				XDUE::Logout();
			}
		});
	}
	return Instance;
}

void XUImpl::BindByType(XUType::LoginType BindType, TFunction<void(bool Success, const FXUError& Error)> CallBack) {
	auto LangModel = XULanguageManager::GetCurrentModel();
	TFunction<void(TSharedPtr<FJsonObject> Paras)> BindBlock = [=](TSharedPtr<FJsonObject> Paras) {
		XUNet::Bind(Paras, [=](TSharedPtr<FXUResponseModel> ResponseModel, FXUError Error) {
			if (!CallBack) {
				return;
			}
			if (ResponseModel.IsValid()) {
				CallBack(true, Error);
			}
			else {
				if (Error.code > 200) {
					CallBack(false, Error);
				}
				else {
					CallBack(false, FXUError(LangModel->tds_bind_error));
				}
			}
		});
	};

	GetAuthParam(BindType, [=](TSharedPtr<FJsonObject> Paras) {
		             BindBlock(Paras);
	             }, [=](FXUError Error) {
		             if (!CallBack) {
			             return;
		             }
		             if (Error.code == 80081) {
			             CallBack(false, FXUError(LangModel->tds_login_cancel));
		             }
		             else {
			             CallBack(false, Error);
		             }
	             });
}

void XUImpl::RequestKidToken(TSharedPtr<FJsonObject> paras,
                             TFunction<void(TSharedPtr<FXUTokenModel> kidToken)> resultBlock,
                             TFunction<void(FXUError error)> ErrorBlock) {
	XUNet::RequestKidToken(paras, [=](TSharedPtr<FXUTokenModel> kidToken, FXUError error) {
		if (error.code == Success && kidToken != nullptr) {
			kidToken->SaveToLocal();
			resultBlock(kidToken);
		}
		else {
			ErrorBlock(error);
		}
	});
}

void XUImpl::RequestUserInfo(bool saveToLocal,
                                   TFunction<void(TSharedPtr<FXUUser> model)> callback,
                                   TFunction<void(FXUError error)> ErrorBlock) {
	XUNet::RequestUserInfo(
		[=](TSharedPtr<FXUUser> user, FXUError error) {
			if (error.code == Success && user != nullptr) {
				if (saveToLocal) {
					user->SaveToLocal();
				}
				callback(user);
			}
			else {
				if (saveToLocal) { // 标记已经失效
					TUDataStorage<FXUStorage>::SaveBool(FXUStorage::TokenInfoIsInvalid, true);
				}
				ErrorBlock(error);
			}
		});

}

void XUImpl::AsyncNetworkTdsUser(const FString& userId,
                                       TFunction<void(FString SessionToken)> callback,
                                       TFunction<void(FXUError error)> ErrorBlock) {
	XUNet::RequestSyncToken(
		[=](TSharedPtr<FXUSyncTokenModel> model, FXUError error) {
			if (error.code == Success && model != nullptr) {
				model->SaveToLocal();
				AsyncLocalTdsUser(userId, model->sessionToken);
				callback(model->sessionToken);
			}
			else {
				ErrorBlock(error);
			}
		}
	);
}

void XUImpl::AsyncLocalTdsUser(const FString& userId, const FString& sessionToken) {
	// LCUser lcUser = LCObject.CreateWithoutData(LCUser.CLASS_NAME, userId) as LCUser;
	// lcUser.SessionToken = token;
	// await lcUser.SaveToLocal();
}


void XUImpl::InitFinish(XUInitCallback CallBack) {
	XUConfigManager::InitTapSDK();
	if (CallBack) {
		CallBack(true, "");
	}
	// 请求服务端 config
	RequestServerConfig();
}

void XUImpl::RequestServerConfig() {
	XUConfigManager::RequestServerConfig();
	XUAgreementManager::RequestServerAgreements(false, nullptr);
	if (XUConfigManager::IsCN()) {
		return;
	}
	XUConfigManager::GetRegionInfo([](TSharedPtr<FXUIpInfoModel> ModelPtr) {
		
	});
}

void XUImpl::LoginSuccess(TSharedPtr<FXUUser> User, TFunction<void(TSharedPtr<FXUUser>)> SuccessBlock) {
	OnLoginSuccess.Broadcast();
	if (SuccessBlock) {
		SuccessBlock(User);
	}
}