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
                         TFunction<void(const FXUUser& User)> resultBlock,
                         TFunction<void(FXUError error)> ErrorBlock) {
	auto lmd = XULanguageManager::GetCurrentModel();
	if (LoginType == XUType::Default) {
		bool TokenInfoIsInvalid = TUDataStorage<FXUStorage>::LoadBool(FXUStorage::TokenInfoIsInvalid);
		if (TokenInfoIsInvalid) { // 如果token已经失效了, 清除登录缓存及协议
			TUDataStorage<FXUStorage>::Remove(FXUStorage::TokenInfoIsInvalid);
			FXUUser::ClearUserData();
			OnTokenIsInvalid.Broadcast();
			if (ErrorBlock) {
				ErrorBlock(FXUError(lmd->tds_login_failed));
			}
			return;
		}
		auto localUser = XDUE::GetUserInfo();
		if (localUser.IsValid()) {
			RequestUserInfo([](TSharedPtr<FXUUser> ModelPtr) {
				                ModelPtr->SaveToLocal();
			                }, nullptr, [](FXUError Error) {
				                TUDataStorage<FXUStorage>::SaveBool(FXUStorage::TokenInfoIsInvalid, true);
			                });
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
		// User和Token保存在一起，等API改版的时候实现
		if (XDUE::GetUserInfo().IsValid()) {
			if (ErrorBlock) {
				ErrorBlock(FXUError("The user is logged in"));
			}
			return;
		}
		GetAuthParam(LoginType, [=](TSharedPtr<FJsonObject> paras) {
			if (FXUTokenModel::GetLocalModel().IsValid()) {
				TUDebuger::WarningLog("Login Token Has Exist");
				FXUTokenModel::ClearToken();
			}
			UTUHUD::ShowWait();
			TFunction<void(FXUError error)> ErrorCallBack = [=](FXUError error) {
				UTUHUD::Dismiss();
				FXUUser::ClearUserData();
				if (ErrorBlock) {
					ErrorBlock(error);
				}
			};
			RequestKidToken(false, paras, [=](TSharedPtr<FXUTokenModel> kidToken) {
				RequestUserInfo([=](TSharedPtr<FXUUser> user) {
					AsyncNetworkTdsUser(user->userId, [=](FString SessionToken) {
						UTUHUD::Dismiss();
						user->SaveToLocal();
						LoginSuccess(user, resultBlock);
					}, ErrorCallBack);
				}, ErrorCallBack, nullptr);
			}, ErrorCallBack);
		}, ErrorBlock);
	}
}

void XUImpl::LoginByConsole(TFunction<void(const FXUUser& User)> SuccessBlock, TFunction<void()> FailBlock,
	TFunction<void(const FXUError& Error)> ErrorBlock) {
	FString SteamPath = GetSteamworksSDKPath();
	if (!SteamPath.IsEmpty()) {
		FString SteamID;
		FString SteamAuth;
		GetSteamInfo(SteamPath, SteamID, SteamAuth);
		
		if (SteamID.IsEmpty()) {
			ErrorBlock(FXUError("SteamID is Empty"));
			TUDebuger::WarningLog(TEXT("SteamID is Empty"));
			return;
		}

		auto XDToken = FXUTokenModel::GetLocalModel();
		auto XDUser = FXUUser::GetLocalModel();
		// 如果有缓存，直接返回User
		if (XDToken.IsValid() && XDUser.IsValid() && !XDToken->ConsoleID.IsEmpty() && XDToken->ConsoleID == SteamID) {
			SuccessBlock(*XDUser.Get());
			RequestUserInfo([](TSharedPtr<FXUUser> ModelPtr) {
				                ModelPtr->SaveToLocal();
			                }, nullptr, [=](FXUError Error) {
				                // token失效，静默重新登录
				                TFunction<void(FXUError error)> ErrorCallBack = [](FXUError error) {
				                };
				                GetAuthParam(XUType::Steam, [=](TSharedPtr<FJsonObject> paras) {
					                RequestKidToken(false, paras, [=](TSharedPtr<FXUTokenModel> kidToken) {
						                RequestUserInfo([=](TSharedPtr<FXUUser> user) {
							                user->SaveToLocal();
							                AsyncNetworkTdsUser(user->userId, nullptr, nullptr);
						                }, ErrorCallBack, nullptr);
					                }, ErrorCallBack);
				                }, ErrorCallBack);
			                });
			TUDebuger::DisplayLog("Steam 缓存登录成功");
			return;
		}
		UTUHUD::ShowWait();
		TFunction<void(FXUError Error)> ErrorCallBack = [=](FXUError Error) {
			UTUHUD::Dismiss();
			FXUUser::ClearUserData();
			if (ErrorBlock) {
				ErrorBlock(Error);
			}
		};
		GetAuthParam(XUType::Steam, [=](TSharedPtr<FJsonObject> paras) {
			RequestKidToken(true, paras, [=](TSharedPtr<FXUTokenModel> kidToken) {
				                RequestUserInfo([=](TSharedPtr<FXUUser> user) {
					                AsyncNetworkTdsUser(user->userId, [=](FString SessionToken) {
						                UTUHUD::Dismiss();
						                user->SaveToLocal();
						                LoginSuccess(user, SuccessBlock);
					                }, ErrorCallBack);
				                }, ErrorCallBack, nullptr);
			                }, [=](FXUError Error) {
				                if (Error.code == 40111 && FailBlock) {
					                UTUHUD::Dismiss();
					                FailBlock();
				                }
				                else {
					                ErrorCallBack(Error);
				                }
			                }, SteamID);
		}, ErrorCallBack);
		return;
	}
	ErrorBlock(FXUError("Not Support Platform"));
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
	else if (LoginType == XUType::Steam) {
		TUDebuger::DisplayLog("Steam Login");
		FString SteamPath = GetSteamworksSDKPath();
		if (SteamPath.IsEmpty()) {
			XUThirdAuthHelper::WebAuth(XUThirdAuthHelper::SteamAuth,
			                           [=](TSharedPtr<FJsonObject> AuthParas) {
				                           AuthParas->SetNumberField("type", (int)LoginType);
				                           resultBlock(AuthParas);
			                           }, ErrorBlock);
		}
		else {
			FString SteamID;
			FString SteamAuth;
			GetSteamInfo(SteamPath, SteamID, SteamAuth);
			if (!SteamID.IsEmpty() && !SteamAuth.IsEmpty()) {
				TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
				JsonObject->SetNumberField("type", (int)LoginType);
				JsonObject->SetStringField("token", SteamAuth);
				resultBlock(JsonObject);
			} else {
				ErrorBlock(FXUError("Steam UserInfo Get Error"));
				TUDebuger::WarningLog(TEXT("Steam UserInfo Get Error"));
			}
		}
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

void XUImpl::RequestKidToken(bool IsConsole, TSharedPtr<FJsonObject> paras,
                             TFunction<void(TSharedPtr<FXUTokenModel> kidToken)> resultBlock,
                             TFunction<void(FXUError error)> ErrorBlock,
                              const FString& ConsoleID) {
	XUNet::RequestKidToken(IsConsole, paras, [=](TSharedPtr<FXUTokenModel> kidToken, FXUError error) {
		if (error.code == Success && kidToken != nullptr) {
			kidToken->ConsoleID = ConsoleID;
			kidToken->SaveToLocal();
			resultBlock(kidToken);
		}
		else {
			ErrorBlock(error);
		}
	});
}

void XUImpl::RequestUserInfo(TFunction<void(TSharedPtr<FXUUser> ModelPtr)> CallBack,
	TFunction<void(FXUError Error)> ErrorBlock, TFunction<void(FXUError Error)> TokenInvalidBlock) {
	XUNet::RequestUserInfo(
	[=](TSharedPtr<FXUUser> user, FXUError error) {
		if (error.code == Success && user != nullptr) {
			if (CallBack) {
				CallBack(user);
			}
		}
		else {
			if (error.IsNetWorkError == false && error.code != Success && TokenInvalidBlock) {
				TokenInvalidBlock(error);
			} else {
				if (ErrorBlock) {
					ErrorBlock(error);
				}
			}
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
				if (callback) {
					callback(model->sessionToken);
				}
			}
			else {
				if (ErrorBlock) {
					ErrorBlock(error);
				}
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
void XUImpl::LoginSuccess(TSharedPtr<FXUUser> UserPtr, TFunction<void(const FXUUser& User)> SuccessBlock) {
	OnLoginSuccess.Broadcast();
	if (SuccessBlock) {
		SuccessBlock(*UserPtr.Get());
	}
}

FString XUImpl::GetSteamworksSDKPath() {
#if WITH_EDITOR
	return "";
#else
	TArray<FString> FindFiles;
	FString Path = FPlatformProcess::BaseDir();
	Path = Path + "../../../Engine/Binaries/ThirdParty/Steamworks";
#if PLATFORM_MAC
	IFileManager::Get().FindFilesRecursive(FindFiles, *Path, TEXT("*.dylib"), true, false);
#elif PLATFORM_WINDOWS
	IFileManager::Get().FindFilesRecursive(FindFiles, *Path, TEXT("*.dll"), true, false);
#endif
	if (FindFiles.Num() > 0) {
		TUDebuger::DisplayShow(FindFiles[0]);
		return FindFiles[0];
	}
	return "";
#endif
}

void XUImpl::GetSteamInfo(const FString& SDKPath, FString& SteamID, FString& SteamAuth) {
	SteamID = "";
	SteamAuth = "";
	auto Handle = FPlatformProcess::GetDllHandle(*SDKPath);
	if (Handle) {
		//清除之前存在的错误
		dlerror();
		
		typedef int32 HSteamPipe;
		typedef int32 HSteamUser;
		typedef uint32 HAuthTicket;
		typedef HSteamUser (*GetHSteamUser)();
		typedef HSteamPipe (*GetHSteamPipe)();
		typedef intptr_t (*CreateInterface)( const char *ver );
		typedef intptr_t (*GetISteamUser)(intptr_t instancePtr, HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char * pchVersion);
		typedef HAuthTicket (*GetAuthSessionTicket)(intptr_t instancePtr, void * pTicket, int cbMaxTicket, uint32 * pcbTicket);
		typedef uint64 (*GetSteamID)(intptr_t instancePtr);
		
		const char * STEAMUSER_INTERFACE_VERSION = "SteamUser020";
		const char * STEAMCLIENT_INTERFACE_VERSION = "SteamClient020";
		
		GetHSteamUser GetHSteamUserFunc = (GetHSteamUser)FPlatformProcess::GetDllExport(Handle, TEXT("GetHSteamUser"));
		GetHSteamPipe GetHSteamPipeFunc = (GetHSteamPipe)FPlatformProcess::GetDllExport(Handle, TEXT("GetHSteamPipe"));
		CreateInterface CreateInterfaceFunc = (CreateInterface)FPlatformProcess::GetDllExport(Handle, TEXT("SteamInternal_CreateInterface"));
		GetISteamUser GetISteamUserFunc = (GetISteamUser)FPlatformProcess::GetDllExport(Handle, TEXT("SteamAPI_ISteamClient_GetISteamUser"));
		GetSteamID GetSteamIDFunc = (GetSteamID)FPlatformProcess::GetDllExport(Handle, TEXT("SteamAPI_ISteamUser_GetSteamID"));
		GetAuthSessionTicket GetAuthSessionTicketFunc = (GetAuthSessionTicket)FPlatformProcess::GetDllExport(Handle, TEXT("SteamAPI_ISteamUser_GetAuthSessionTicket"));

		if (GetHSteamUserFunc &&
			GetHSteamPipeFunc &&
			CreateInterfaceFunc &&
			GetISteamUserFunc &&
			GetAuthSessionTicketFunc) {
			TUDebuger::DisplayShow("funcs are exits");
			auto user = GetHSteamUserFunc();
			auto pipe = GetHSteamPipeFunc();
			auto steamClient = CreateInterfaceFunc(STEAMCLIENT_INTERFACE_VERSION);
			auto steamUser = GetISteamUserFunc(steamClient, user, pipe, STEAMUSER_INTERFACE_VERSION);
			uint8 AuthToken[1024];
			uint32 AuthTokenSize = 0;
			GetAuthSessionTicketFunc(steamUser, AuthToken, UE_ARRAY_COUNT(AuthToken), &AuthTokenSize);
			SteamAuth = BytesToHex(AuthToken, AuthTokenSize);
			SteamID = FString::Printf(TEXT("%lld"), GetSteamIDFunc(steamUser));
			TUDebuger::DisplayShow("SteamAuth: " + SteamAuth);
			TUDebuger::DisplayShow("SteamID: " + SteamID);
		}
		else {
			TUDebuger::WarningShow("steam funcs are not exits");
		}
		FPlatformProcess::FreeDllHandle(Handle);
	} else {
		TUDebuger::WarningShow("no steam dylib");
	}
}
