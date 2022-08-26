#include "AntiAddictionUE.h"

#include "TUDebuger.h"
#include "Server/AAUImpl.h"
#include "Server/AAUStorage.h"
#include "UI/AAUHealthTipWidget.h"

AntiAddictionUE::SimpleDelegate AntiAddictionUE::OnExit;

AntiAddictionUE::SimpleDelegate AntiAddictionUE::OnSwitchAccount;

void AntiAddictionUE::Init(const AAUType::Config& Config) {
	AAUImpl::Get()->Init(Config);
}

void AntiAddictionUE::Login(const FString& UserID, TFunction<void(bool Result, const FString& Msg)> CallBack) {
	if (UserID.IsEmpty()) {
		TUDebuger::ErrorLog("AntiAddiction UserID is Empty");
		return;
	}
	AAUImpl::Get()->Login(UserID, CallBack);
}

void AntiAddictionUE::EnterGame() {
	AAUImpl::Get()->EnterGame();
}

void AntiAddictionUE::LeaveGame() {
	AAUImpl::Get()->LeaveGame();
}

void AntiAddictionUE::Logout() {
	AAUImpl::Get()->Logout();
}

int AntiAddictionUE::GetCurrentUserAgeLimit() {
	return AAUImpl::Get()->GetCurrentUserAgeLimit();
}

int AntiAddictionUE::GetCurrentUserRemainTime() {
	return AAUImpl::Get()->GetCurrentUserRemainTime();
}

void AntiAddictionUE::CheckPayLimit(int Amount,TFunction<void(bool Status)> CallBack,TFunction<void(const FString& Msg)> FailureHandler) {
	AAUImpl::Get()->CheckPayLimit(Amount, CallBack, FailureHandler);
}

void AntiAddictionUE::SubmitPayResult(int Amount, TFunction<void(bool Success)> CallBack,
	TFunction<void(const FString& Msg)> FailureHandler) {
	AAUImpl::Get()->SubmitPayResult(Amount, CallBack, FailureHandler);
}

void AntiAddictionUE::Test() {
	int Temp = TUDataStorage<FAAUStorage>::LoadNumber("MyTest");
	TUDebuger::DisplayShow(FString::Printf(TEXT("num: %i"), Temp));
	Temp += 1;
	TUDataStorage<FAAUStorage>::SaveNumber("MyTest", Temp);
}
