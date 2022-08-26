#include "AAUImpl.h"

#include "AAUHelper.h"
#include "AAUNet.h"
#include "AAUServer.h"
#include "AAUStorage.h"
#include "AntiAddictionUE.h"
#include "TUDebuger.h"
#include "TUHUD.h"
#include "Model/AAUUser.h"
#include "UI/AAUHealthTipWidget.h"
#include "UI/AAUManualRealNameWidget.h"
#include "UI/AAURealNameSuccessWidget.h"
#include "UI/AAUTipWidget.h"


TSharedPtr<AAUImpl> AAUImpl::Instance = nullptr;
FString AAUImpl::LocalTokenString = "standalone_anti_addiction_token";

TSharedPtr<AAUImpl>& AAUImpl::Get() {
	if (!Instance.IsValid()) {
		Instance = MakeShareable(new AAUImpl);
		Instance->Server = MakeShareable(new AAUServer);
		Instance->Server->Delegate = Instance.Get();
	}
	return Instance;
}

void AAUImpl::Init(const AAUType::Config& _Config) {
	Config = _Config;
	AAUNet::GetSDKConfig([](TSharedPtr<FAAUConfigModel> ModelPtr, const FTUError& Error) {
		if (ModelPtr.IsValid()) {
			FAAUConfigModel::SaveToLocal(ModelPtr);
		} else {
			TUDebuger::WarningLog(FString::Printf(TEXT("Request AA Config Fail: %s"), *Error.GetErrorReason()));
		}
	});
}

void AAUImpl::Login(const FString& UserID, TFunction<void(bool Result, const FString& Msg)> CallBack) {
	CurrentUserID = UserID;
	CurrentStartUpCallBack = CallBack;
	TSharedPtr<FAAUUser> LoginUser = TUDataStorage<FAAUStorage>::LoadStruct<FAAUUser>(FAAUStorage::HasLoginedUser + UserID);

	UTUHUD::ShowWait();
	auto ResultBlock = [=](TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error) {
		UTUHUD::Dismiss();
		if (ModelPtr.IsValid()) {
			switch (ModelPtr->GetAuthState()) {
			case FAAURealNameResultModel::Success: {
				Login(ModelPtr->anti_addiction_token, ModelPtr->age_limit);
			}
				break;
			case FAAURealNameResultModel::Verifying:
				ShowRealNameUI(nullptr, AAURealNameWordTypeVerifying);
				break;
			case FAAURealNameResultModel::Fail:
				if (ModelPtr->has_auth_record) {
					ShowRealNameUI(nullptr, AAURealNameWordTypeVerifyFail);
				} else {
					ShowRealNameUI(nullptr, AAURealNameWordTypeVerify);
				}
				break;
			}
		} else {
			IsNeedStandAlone([=](bool IsNeed) {
				if (LoginUser.IsValid() && !LoginUser->AccessToken.IsEmpty()) {
					if (LoginUser->AccessToken == LocalTokenString && !IsNeed) {
						// TUDebuger::DisplayLog(FString::Printf(TEXT("haha:%s - %d"), *LoginUser->AccessToken, IsNeed));
						TryAgainStartUp(Error.GetErrorReason());
						return;
					}
					Login(LoginUser->AccessToken, LoginUser->AgeLimit);
				}
				else {
					if (IsNeed) {
						ShowRealNameUI(nullptr, AAURealNameWordTypeVerify);
						return;
					}
					TryAgainStartUp(Error.GetErrorReason());
				}
			});
		}
	};
	
	if (LoginUser.IsValid() && LoginUser->AccessToken == LocalTokenString) {
		FString SubmitCardName = TUDataStorage<FAAUStorage>::LoadString(FAAUStorage::SubmitCardName + UserID);
		FString SubmitCardID = TUDataStorage<FAAUStorage>::LoadString(FAAUStorage::SubmitCardID + UserID);
		if (!SubmitCardName.IsEmpty() && !SubmitCardID.IsEmpty()) {
			ManualVerify(UserID, SubmitCardName, SubmitCardID, ResultBlock);
			return;
		}
	}
	CheckRealNameState(UserID, ResultBlock);
}

void AAUImpl::ManualVerify(const FString& UserID, const FString& Name, const FString& CardID,
                           TFunction<void(TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error)>
                           CallBack) {
	auto StandAloneVerifyBlock = [=]() {
		if (!CallBack) {
			return;
		}
		if (!AAUHelper::ValidateCardID(CardID) || Name.IsEmpty()) {
			CallBack(nullptr, FTUError(-1, TEXT("身份证号码非法")));
			return;
		}
		const int Age = AAUHelper::GetAge(CardID);
		TSharedPtr<FAAURealNameResultModel> ModelPtr = MakeShareable(new FAAURealNameResultModel);
		if (Age >= 0 && Age < 8) {
			ModelPtr->age_limit = AAUType::Child;
		}
		else if (Age >= 8 && Age < 16) {
			ModelPtr->age_limit = AAUType::Teen;
		}
		else if (Age >= 16 && Age < 18) {
			ModelPtr->age_limit = AAUType::Young;
		}
		else if (Age >= 18 && Age < 200) {
			ModelPtr->age_limit = AAUType::Adult;
		}
		else {
			CallBack(nullptr, FTUError(-1, TEXT("身份证号码非法")));
			return;
		}
		ModelPtr->anti_addiction_token = LocalTokenString;
		ModelPtr->status = FAAURealNameResultModel::Success;
		CallBack(ModelPtr, FTUError());
	};
	
	if (IsNeedStandAlone()) {
		StandAloneVerifyBlock();
		return;
	}
	AAUNet::ManualVerify(UserID, Name, CardID, [=](TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error) {
		if (!CallBack) {
			return;
		}
		if (ModelPtr.IsValid()) {
			CallBack(ModelPtr, Error);
		} else {
			IsNeedStandAlone([=](bool IsNeed) {
				if (IsNeed) {
					StandAloneVerifyBlock();
				} else {
					CallBack(ModelPtr, Error);
				}
			});
		}
	});
}

void AAUImpl::CheckRealNameState(const FString& UserID,
	TFunction<void(TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error)> CallBack) {
	AAUNet::CheckRealNameState(UserID, CallBack);
}

bool AAUImpl::IsNeedStandAlone(TFunction<void(bool IsNeed)> CallBack) {
	if (AAUNet::RealNameServerIsCrash == false) {
		if (CallBack) {
			CallBack(false);
		}
		return false;
	}
	bool Enable = true;   // 目前比较宽松，尽量放开
	if (StandAloneEnable.IsValid()) {
		if (CallBack) {
			CallBack(*StandAloneEnable.Get());
		}
		return *StandAloneEnable.Get();
	}
	else {
		AAUNet::GetStaticSetting([=](TSharedPtr<FAAUSettingsModel> ModelPtr, const FTUError& Error) {
			if (ModelPtr.IsValid()) {
				StandAloneEnable = MakeShareable(new bool(ModelPtr->stand_alone_mode));
				if (CallBack) {
					CallBack(ModelPtr->stand_alone_mode);
				}
			}
			else {
				TUDebuger::WarningLog(
					FString::Printf(TEXT("Request Static Setting Fail: %s"), *Error.GetErrorReason()));
				if (CallBack) {
					CallBack(Enable);
				}
			}
		});
	}
	return Enable;
}

void AAUImpl::EnterGame() {
	Server->EnterGame();
}

void AAUImpl::LeaveGame() {
	Server->LeaveGame();
}

void AAUImpl::Logout() {
	Server->Logout();
}

int AAUImpl::GetCurrentUserAgeLimit() {
	return Server->GetAgeLimit();
}

int AAUImpl::GetCurrentUserRemainTime() {
	return Server->GetRemainTime();
}

void AAUImpl::CheckPayLimit(int Amount, TFunction<void(bool Status)> CallBack, TFunction<void(const FString& Msg)> FailureHandler) {
	Server->CheckPayLimit(Amount, [=](bool Status, const FString& Title, const FString& Description) {
		if (CallBack) {
			CallBack(Status);
		}
		if (Status == false) {
			auto Widget = UAAUHealthTipWidget::ShowUI(Title, Description, TEXT("返回游戏"));
			Widget->HiddenSwitchAccountBtn();
		}
	}, FailureHandler);
}

void AAUImpl::SubmitPayResult(int Amount, TFunction<void(bool Success)> CallBack,
	TFunction<void(const FString& Msg)> FailureHandler) {
	Server->SubmitPayResult(Amount, CallBack, FailureHandler);
}

void AAUImpl::PerformStartUpCallBack(bool Result, const FString& Msg) {
	if (CurrentStartUpCallBack == nullptr) {
		return;
	}
	CurrentStartUpCallBack(Result, Msg);
	CurrentStartUpCallBack = nullptr;
}

void AAUImpl::KickOut(AAUTimeBoundary Boundary, const FString& Title, const FString& Content) {
	if (Boundary == AAUTimeBoundaryCurfew) {
		ShowHealthTipUI(Title, Content, 0, AAUHealthTipTypeInCurfew);
	} else if (Boundary == AAUTimeBoundaryTimeLimit) {
		ShowHealthTipUI(Title, Content, 0, AAUHealthTipTypeTimeout);
	}
}

void AAUImpl::Login(const FString& AccessToken, int AgeLimit, bool IsFirst) {
	FAAUUser User;
	User.AccessToken = AccessToken;
	User.AgeLimit = AgeLimit;
	User.UserID = CurrentUserID;
	TUDataStorage<FAAUStorage>::SaveStruct(FAAUStorage::HasLoginedUser + CurrentUserID, User);
	auto LoginBlock = [=]() {
		UTUHUD::ShowWait();
		Server->Login(User, [=](const AAULoginResult& Result) {
			UTUHUD::Dismiss();
			if (Result.LoginState == AAULoginResult::SuccessWithLimit || Result.LoginState ==
				AAULoginResult::SuccessWithNoLimit) {
				PerformStartUpCallBack(true, "Success");
				if (Result.LoginState == AAULoginResult::SuccessWithLimit) {
					ShowHealthTipUI(Result.Title, Result.Description, Result.RemainTime, AAUHealthTipTypeFirstLogin);
				}
			}
			else {
				if (Result.LoginState == AAULoginResult::FailWithCurfew) {
					ShowHealthTipUI(Result.Title, Result.Description, Result.RemainTime, AAUHealthTipTypeInCurfew);
				}
				else if (Result.LoginState == AAULoginResult::FailWithTimeout) {
					ShowHealthTipUI(Result.Title, Result.Description, Result.RemainTime, AAUHealthTipTypeTimeout);
				}
				else {
					UTUHUD::ShowToast(TEXT("防沉迷进入失败"));
				}
				PerformStartUpCallBack(false, "Time Limit");
			}
		});
	};
	if (IsFirst) {
		UAAURealNameSuccessWidget::ShowTip(LoginBlock);
	} else {
		LoginBlock();
	}
	
}

void AAUImpl::ShowRealNameUI(UAAUManualRealNameWidget *Widget, AAURealNameWordType Type) {
	if (Widget == nullptr) {
		Widget = UAAUManualRealNameWidget::ShowUI(Type);
	} else {
		Widget->ChangeType(Type);
	}
	Widget->CloseBlock = [=]() {
		PerformStartUpCallBack(false, "Cancel");
	};
	if (Type == AAURealNameWordTypeVerifying) {
		Widget->SubmitBlock = [=](const FString& Name, const FString& CardID) {
			PerformStartUpCallBack(false, "RealName Verifying");
			Widget->Dismiss();
		};
	} else if(Type == AAURealNameWordTypeVerify || Type == AAURealNameWordTypeVerifyFail) {
		Widget->SubmitBlock = [=](const FString& Name, const FString& CardID) {
			if (Name.IsEmpty()) {
				Widget->ShowError(TEXT("姓名不能为空"));
				return;
			}
			if (CardID.IsEmpty()) {
				Widget->ShowError(TEXT("身份信息不能为空"));
				return;
			}
			UTUHUD::ShowWait();
			ManualVerify(CurrentUserID, Name, CardID, [=](TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error) {
				UTUHUD::Dismiss();
				if (ModelPtr.IsValid()) {
					switch (ModelPtr->GetAuthState()) {
					case FAAURealNameResultModel::Success: {
						// 判断是否是单机校验的身份证，如果是，缓存身份证给下次登录校验
						if (ModelPtr->anti_addiction_token == LocalTokenString) {
							TUDataStorage<FAAUStorage>::SaveString(FAAUStorage::SubmitCardName + CurrentUserID, Name);
							TUDataStorage<FAAUStorage>::SaveString(FAAUStorage::SubmitCardID + CurrentUserID, CardID);
						}
						Login(ModelPtr->anti_addiction_token, ModelPtr->age_limit, true);
						Widget->Dismiss();
					}
					break;
					case FAAURealNameResultModel::Verifying:
						ShowRealNameUI(Widget, AAURealNameWordTypeVerifying);
						break;
					case FAAURealNameResultModel::Fail:
						Widget->ShowError(TEXT("认证未通过，请提交真实信息"));
						break;
					}
				}
				else {
					Widget->ShowError(Error.GetErrorReason());
				}
			});
		};
	}
}

void AAUImpl::TryAgainStartUp(const FString& ErrMsg) {
	FString Msg = TEXT("网络错误，请重试");
	if (!ErrMsg.IsEmpty()) {
		Msg = ErrMsg;
	}
	UAAUTipWidget::ShowTip(Msg, TEXT("重试"), [=]() {
		Login(CurrentUserID, CurrentStartUpCallBack);
	});
}

void AAUImpl::ShowHealthTipUI(const FString& Title, const FString& Content, int RemainTime, AAUHealthTipType Type) {
	if (Type == AAUHealthTipTypeTimeout || Type == AAUHealthTipTypeInCurfew) {
		auto Widget = UAAUHealthTipWidget::ShowUI(Title, Content, TEXT("退出游戏"));
		Widget->SwitchAccountBlock = [=]() {
			AntiAddictionUE::OnSwitchAccount.Broadcast();
		};
		Widget->ComformBlock = [=]() {
			AntiAddictionUE::OnExit.Broadcast();
#if WITH_EDITOR
			GEngine->Exec(nullptr, TEXT("QUIT_EDITOR"), *GLog);
#else
			FGenericPlatformMisc::RequestExit(false);
#endif
		};
	} else if (Type == AAUHealthTipTypeFirstLogin) {
		FString Temp = Content.Replace(TEXT("# ${remaining} #"), *FString::Printf(TEXT("%i"), RemainTime/60));
		auto Widget = UAAUHealthTipWidget::ShowUI(Title, Temp, TEXT("进入游戏"));
		Widget->HiddenSwitchAccountBtn();
	}

}



