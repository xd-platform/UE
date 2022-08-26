#include "AAUServer.h"

#include "AAUHelper.h"
#include "AAUImpl.h"
#include "AAUNet.h"
#include "AAUStorage.h"
#include "TUDataStorage.h"
#include "TUDebuger.h"
#include "TUSettings.h"


static int64 CheckTimeSpan = 120;
static int AdultRemainTime = 9999;

void AAUServer::Login(const FAAUUser& User, TFunction<void(const AAULoginResult& Result)> CallBack) {
	if (CallBack == nullptr) {
		TUDebuger::ErrorLog("AAUServer::Login CallBack can not be nullptr");
		return;
	}
	ResetValues();

	AAUHelper::GetTimeSpanWithSeverTime([=](int64 _TimeSpan) {
		this->TimeSpan = _TimeSpan;
		// 走单机验证
		if (AAUImpl::Get()->IsNeedStandAlone()) {
			LoginStandAlone(User, CallBack);
		} else {
			AAUNet::CheckPlayable(User.UserID, User.AccessToken, {}, {}, [=](TSharedPtr<FAAUPlayableModel> ModelPtr,  const FTUError& Error) {
				if (ModelPtr.IsValid()) {
					AAULoginResult Result;
					Result.Title = ModelPtr->title;
					Result.Description = ModelPtr->description;
					Result.RemainTime = ModelPtr->remain_time;
					this->RemainTime = ModelPtr->remain_time;
					// this->CostTime = ModelPtr->cost_time;

					if (User.AgeLimit == AAUType::Adult) {
						Result.LoginState = AAULoginResult::SuccessWithNoLimit;
						CallBack(Result);
						LoginSuccess(User, false);
					} else {
						if (ModelPtr->remain_time > 0) {
							Result.LoginState = AAULoginResult::SuccessWithLimit;
							CallBack(Result);
							LoginSuccess(User, true);
						}
						else {
							if (ModelPtr->restrict_type == 1) {
								Result.LoginState = AAULoginResult::FailWithCurfew;
								CallBack(Result);
							}
							else {
								Result.LoginState = AAULoginResult::FailWithTimeout;
								CallBack(Result);
							}
						}
					}
				}
				else {
					if (Error.code == 16) { // 服务强制踢出
						AAULoginResult Result;
						Result.LoginState = AAULoginResult::FailWithOther;
						CallBack(Result);
						return;
					}
					LoginStandAlone(User, CallBack);
				}
			}, true);
		}
	});
	
}

void AAUServer::EnterGame() {
	if (!FAAUUser::CurrentUser.IsValid()) {
		return;
	}
	if (IsInGame == true) {
		return;
	}
	IsInGame = true;
	TimeRecordBegin = GetCurrentTime();
	TimeRecordEnd = TimeRecordBegin;
	AAUTimeBoundary Boundary;
	RemainTime = CalculateRemainTime(Boundary);
	StartTimer();
}

void AAUServer::LeaveGame() {
	if (!FAAUUser::CurrentUser.IsValid()) {
		return;
	}
	if (IsInGame == false) {
		return;
	}
	IsInGame = false;
	UpdateCostAndRemainTime();// 离开游戏时，更新下时长
	UploadTime(true);// 给服务端更新下时长
	StopTimer();
	
}

void AAUServer::Logout() {
	if (!FAAUUser::CurrentUser.IsValid()) {
		return;
	}
	LeaveGame();
	FAAUUser::CurrentUser = nullptr;
	AAUNet::RealNameServerIsCrash = false;
}

void AAUServer::KickOut(AAUTimeBoundary Boundary, const FString& Title, const FString& Content, bool IsServer) {

	if (!IsServer) {// 如果不是服务接口踢出的，那么给服务端尝试更新下时长
		UploadTime(true);
	}
	IsInGame = false;
	StopTimer();
	FAAUUser::CurrentUser = nullptr;
	AAUNet::RealNameServerIsCrash = false;
	if (Delegate) {
		Delegate->KickOut(Boundary, Title, Content);
	}
}

void AAUServer::ResetValues() {
	IsInGame = false;
	PlayableServerCrashCount = 0;
	CostTime = 0;
	RemainTime = 0;
	TimeRecordBegin = 0;
	TimeRecordEnd = 0;
	StopTimer();
	CostTimes.Empty();
}

int AAUServer::GetRemainTime() {
	if (!FAAUUser::CurrentUser.IsValid()) {
		return 0;
	}
	if (FAAUUser::CurrentUser->AgeLimit == AAUType::Adult) {
		return AdultRemainTime;
	}
	UpdateCostAndRemainTime();
	return RemainTime;
}

int AAUServer::GetAgeLimit() {
	if (!FAAUUser::CurrentUser.IsValid()) {
		return 0;
	}
	return FAAUUser::CurrentUser->AgeLimit;
}

void AAUServer::CheckPayLimit(int Amount,
	TFunction<void(bool Status, const FString& Title, const FString& Description)> CallBack,
	TFunction<void(const FString& Msg)> FailureHandler) {
	if (!FAAUUser::CurrentUser.IsValid()) {
		if (FailureHandler) {
			FailureHandler(TEXT("还未登录，请先登录"));
		}
		return;
	}
	if (FAAUUser::CurrentUser->AccessToken == AAUImpl::LocalTokenString) {
		if (FailureHandler) {
			FailureHandler(TEXT("当前服务异常，无法进行支付"));
		}
		return;
	}
	AAUNet::CheckPayment(Amount, [=](TSharedPtr<FAAUPayableModel> ModelPtr, const FTUError& Error) {
		if (ModelPtr.IsValid()) {
			if (CallBack) {
				CallBack(ModelPtr->status, ModelPtr->title, ModelPtr->description);
			}
		}
		else {
			if (FailureHandler) {
				FailureHandler(Error.error_description);
			}
		}
	});
}

void AAUServer::SubmitPayResult(int Amount, TFunction<void(bool Success)> CallBack,
	TFunction<void(const FString& Msg)> FailureHandler) {
	if (!FAAUUser::CurrentUser.IsValid()) {
		if (FailureHandler) {
			FailureHandler(TEXT("还未登录，请先登录"));
		}
		return;
	}
	if (FAAUUser::CurrentUser->AccessToken == AAUImpl::LocalTokenString) {
		if (FailureHandler) {
			FailureHandler(TEXT("当前服务异常，无法进行支付"));
		}
		return;
	}
	AAUNet::SetPayment(Amount, [=](TSharedPtr<FAAUPaymentModel> ModelPtr, const FTUError& Error) {
		if (ModelPtr.IsValid()) {
			if (CallBack) {
				CallBack(true);
			}
		}
		else {
			if (FailureHandler) {
				FailureHandler(Error.error_description);
			}
		}
	});
}

FDateTime AAUServer::GetCurrentTime() {
	return AAUHelper::GetChinaCurrentTime() + TimeSpan * ETimespan::TicksPerSecond;
}

int64 AAUServer::CalculateRemainTime(AAUTimeBoundary& Boundary) {
	// 确定是成年人不用计算时长
	if (FAAUUser::CurrentUser.IsValid() && FAAUUser::CurrentUser->GetAgeLimit() == AAUType::Adult) {
		Boundary = AAUTimeBoundaryNoLimit;
		return AdultRemainTime;
	}
	// 宵禁时间。
	auto IntervalForCurfew = AAUHelper::IntervalForCurfew(GetCurrentTime());
	if (IntervalForCurfew <= 0) {
		Boundary = AAUTimeBoundaryCurfew;
		return 0;
	}
	bool IsHoliday = AAUHelper::IsHoliday(GetCurrentTime());
	auto Config = FAAUConfigModel::GetLocalModel();
	
	int64 TotolTime = IsHoliday ? Config->child_protected_config.child_holiday_time : Config->child_protected_config.child_common_time;
	// 如果可玩时间为0，说明当天是宵禁。
	if (TotolTime <= 0 ) {
		Boundary = AAUTimeBoundaryCurfew;
		return 0;
	}
	
	int64 TempRemainTime = TotolTime - this->CostTime;
	if (TempRemainTime < IntervalForCurfew) {
		Boundary = AAUTimeBoundaryTimeLimit;
		return TempRemainTime;
	} else {
		Boundary = AAUTimeBoundaryCurfew;
		return IntervalForCurfew;
	}
}

void AAUServer::LoginStandAlone(const FAAUUser& User, TFunction<void(const AAULoginResult& Result)> CallBack) {
	if (User.AgeLimit == AAUType::Adult) {
		AAULoginResult Result;
		Result.LoginState = AAULoginResult::SuccessWithNoLimit;
		CallBack(Result);
		LoginSuccess(User, false);
	} else {
		auto Today = GetCurrentTime();
		FString TodayStr = FString::Printf(TEXT("%04i%02i%02i"), Today.GetYear(), Today.GetMonth(), Today.GetDay());
		int64 LastLoginTimestamp = TUDataStorage<FAAUStorage>::LoadNumber(FAAUStorage::LastLoginTime + User.UserID);
		FDateTime LastLoginDay = FDateTime::FromUnixTimestamp(LastLoginTimestamp);
		FString LastLoginDayStr = FString::Printf(TEXT("%04i%02i%02i"), LastLoginDay.GetYear(), LastLoginDay.GetMonth(), LastLoginDay.GetDay());
		if (TodayStr == LastLoginDayStr) {
			this->CostTime = TUDataStorage<FAAUStorage>::LoadNumber(FAAUStorage::LastCostTime + User.UserID);
		}
		AAUTimeBoundary Boundary;
		RemainTime = CalculateRemainTime(Boundary);
		AAULoginResult Result;
		if (RemainTime > 0) {
			Result.LoginState = AAULoginResult::SuccessWithLimit;
			auto Tip = FAAUConfigModel::GetLocalModel()->GetNonageHealthTip(AAUHealthTipTypeFirstLogin);
			Result.Title = Tip.title;
			Result.Description = Tip.description;
			Result.RemainTime = RemainTime;
			CallBack(Result);
			// 已经算过了。就不需要在算costtime了。
			LoginSuccess(User, false);
		}
		else {
			if (Boundary == AAUTimeBoundaryCurfew) {
				Result.LoginState = AAULoginResult::FailWithCurfew;
				auto Tip = FAAUConfigModel::GetLocalModel()->GetNonageHealthTip(AAUHealthTipTypeInCurfew);
				Result.Title = Tip.title;
				Result.Description = Tip.description;
			} else {
				Result.LoginState = AAULoginResult::FailWithTimeout;
				auto Tip = FAAUConfigModel::GetLocalModel()->GetNonageHealthTip(AAUHealthTipTypeTimeout);
				Result.Title = Tip.title;
				Result.Description = Tip.description;
			}
			CallBack(Result);
		}
	}
}


void AAUServer::LoginSuccess(const FAAUUser& User, bool NeedLoadCostTime) {
	auto LoginTime = GetCurrentTime();
	FAAUUser::CurrentUser = MakeShareable(new FAAUUser(User));
	// TUDataStorage<FAAUStorage>::SaveNumber(FAAUStorage::LastCostTime + User.UserID, CostTime);
	if (NeedLoadCostTime) {
		auto Today = GetCurrentTime();
		FString TodayStr = FString::Printf(TEXT("%04i%02i%02i"), Today.GetYear(), Today.GetMonth(), Today.GetDay());
		int64 LastLoginTimestamp = TUDataStorage<FAAUStorage>::LoadNumber(FAAUStorage::LastLoginTime + User.UserID);
		FDateTime LastLoginDay = FDateTime::FromUnixTimestamp(LastLoginTimestamp);
		FString LastLoginDayStr = FString::Printf(TEXT("%04i%02i%02i"), LastLoginDay.GetYear(), LastLoginDay.GetMonth(), LastLoginDay.GetDay());
		if (TodayStr == LastLoginDayStr) {
			this->CostTime = TUDataStorage<FAAUStorage>::LoadNumber(FAAUStorage::LastCostTime + User.UserID);
		} else {
			this->CostTime = 0;
		}
	}
	TUDataStorage<FAAUStorage>::SaveNumber(FAAUStorage::LastLoginTime + User.UserID, LoginTime.ToUnixTimestamp());
	EnterGame();
}

void AAUServer::StartCheckTimer() {
	auto User = FAAUUser::CurrentUser;
	if (!User.IsValid()) {
		return;
	}
	TUSettings::GetGameInstance()->GetTimerManager().SetTimer(CheckTimer, [=]() {
		UpdateCostAndRemainTime();
		if (AAUImpl::Get()->IsNeedStandAlone() || !FAAUConfigModel::GetLocalModel()->upload_user_action) {
			if (User->AgeLimit != AAUType::Adult && RemainTime <= CheckTimeSpan) {
				StartCountDownTimer();
			}
		}
		else {
			UploadTime(false);
		}
	}, CheckTimeSpan, true);
}

void AAUServer::StartCountDownTimer() {
	auto User = FAAUUser::CurrentUser;
	if (!User.IsValid()) {
		return;
	}
	TUSettings::GetGameInstance()->GetTimerManager().SetTimer(CountDownTimer, [=]() {
		if (User->AgeLimit == AAUType::Adult) {
			
		}
		else {
			UpdateCostAndRemainTime();
			if (AAUImpl::Get()->IsNeedStandAlone() || !FAAUConfigModel::GetLocalModel()->upload_user_action || PlayableServerCrashCount >= 3) {
				AAUTimeBoundary Boundary;
				RemainTime = CalculateRemainTime(Boundary);
		
				if (RemainTime <= 0) {
						if (Boundary == AAUTimeBoundaryTimeLimit) {
							auto Tip = FAAUConfigModel::GetLocalModel()->GetNonageHealthTip(AAUHealthTipTypeTimeout);
							KickOut(AAUTimeBoundaryTimeLimit, Tip.title, Tip.description, false);
						}
						else {
							auto Tip = FAAUConfigModel::GetLocalModel()->GetNonageHealthTip(AAUHealthTipTypeInCurfew);
							KickOut(AAUTimeBoundaryCurfew, Tip.title, Tip.description, false);
						}
				} else {
					StartCountDownTimer();
				}
			}
			else {
				UploadTime(false);
			}
		}
	}, FMath::Max<int>(1, RemainTime), false);
}

void AAUServer::StopTimer() {
	if (CheckTimer.IsValid()) {
		TUSettings::GetGameInstance()->GetTimerManager().ClearTimer(CheckTimer);
	}
	CheckTimer.Invalidate();

	if (CountDownTimer.IsValid()) {
		TUSettings::GetGameInstance()->GetTimerManager().ClearTimer(CountDownTimer);
	}
	CountDownTimer.Invalidate();
}

void AAUServer::StartTimer() {
	StartCheckTimer();
	if (RemainTime <= CheckTimeSpan) {
		StartCountDownTimer();
	}
}

void AAUServer::UploadTime(bool IsPure) {
	static bool IsUploading = false;
	if (IsUploading) {
		return;
	}
	IsUploading = true;
	
	auto User = FAAUUser::CurrentUser;
	if (CostTimes.Num() <= 0) {
		TArray<int> SendTimeInterval;
		SendTimeInterval.Add(TimeRecordBegin.ToUnixTimestamp());
		SendTimeInterval.Add(TimeRecordEnd.ToUnixTimestamp());
		CostTimes.Add(SendTimeInterval);
	} else {
		auto LastSendTimeInterval = CostTimes.Last();
		if (LastSendTimeInterval[1] >= TimeRecordBegin.ToUnixTimestamp()) {
			LastSendTimeInterval[1] = TimeRecordEnd.ToUnixTimestamp();
		} else  {
			TArray<int> SendTimeInterval;
			SendTimeInterval.Add(TimeRecordBegin.ToUnixTimestamp());
			SendTimeInterval.Add(TimeRecordEnd.ToUnixTimestamp());
			CostTimes.Add(SendTimeInterval);
		}
	}
	
	auto SendTime = GetCurrentTime();
	AAUNet::CheckPlayable(User->UserID, User->AccessToken, CostTimes, CostTimes, [=](TSharedPtr<FAAUPlayableModel> ModelPtr,  const FTUError& Error) {
		IsUploading = false;
		if (ModelPtr.IsValid()) {
			PlayableServerCrashCount = 0;
			if (ModelPtr->status > 0) {
				// 清空所有时间记录，下次发送新的
				this->CostTimes.Empty();
				TimeRecordBegin = SendTime;
				TimeRecordEnd = SendTime;
			}
			this->RemainTime = ModelPtr->remain_time;
			// this->CostTime = ModelPtr->cost_time;
			if (IsPure) {
				return;
			}

			if (User->AgeLimit != AAUType::Adult) {
				// 踢出
				if (RemainTime <= 0) {
					if (ModelPtr->restrict_type == 1) {
						KickOut(AAUTimeBoundaryCurfew, ModelPtr->title, ModelPtr->description, true);
					}
					else {
						KickOut(AAUTimeBoundaryTimeLimit, ModelPtr->title, ModelPtr->description, true);
					}
				}
				else if (this->RemainTime <= CheckTimeSpan) {
					StartCountDownTimer();
				}
			}
		}
		else {
			PlayableServerCrashCount++;
			if (IsPure) {
				return;
			}
			if (Error.code == 16) {
				// 服务强制踢出
				KickOut(AAUTimeBoundaryNoLimit, TEXT("健康提醒"), Error.GetErrorReason(), true);
				return;
			}
			StartCountDownTimer();
		}
	}, false);
}

void AAUServer::UpdateCostAndRemainTime() {
	auto Now = GetCurrentTime();
	auto Span = (Now - TimeRecordEnd).GetTotalSeconds();
	TimeRecordEnd = Now;
	RemainTime -= Span;
	CostTime += Span;

	if (FAAUUser::CurrentUser.IsValid() && FAAUUser::CurrentUser->AgeLimit != AAUType::Adult) {
		// 存下用户的耗时时间
		TUDataStorage<FAAUStorage>::SaveNumber(FAAUStorage::LastLoginTime + FAAUUser::CurrentUser->UserID, TimeRecordEnd.ToUnixTimestamp());
		TUDataStorage<FAAUStorage>::SaveNumber(FAAUStorage::LastCostTime + FAAUUser::CurrentUser->UserID, CostTime);
	}
	
}
