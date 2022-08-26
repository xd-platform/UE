#pragma once
#include "AAUServerDelegate.h"
#include "AAUType.h"
#include "TUError.h"
#include "Model/AAURealNameResultModel.h"
#include "UI/AAUManualRealNameWidget.h"

class AAUServer;
class AAUImpl: AAUServerDelegate {
public:
	static TSharedPtr<AAUImpl>& Get();

	AAUType::Config Config;

	static FString LocalTokenString;

	void Init(const AAUType::Config& _Config);

	void Login(const FString& UserID, TFunction<void(bool Result, const FString& Msg)> CallBack);
	
	// MARK: - 实名认证部分
	void ManualVerify(const FString& UserID, const FString& Name, const FString& CardID,
		TFunction<void(TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error)> CallBack);

	void CheckRealNameState(const FString& UserID, TFunction<void(TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error)> CallBack);

	bool IsNeedStandAlone(TFunction<void(bool IsNeed)> CallBack = nullptr);

	void EnterGame();

	void LeaveGame();

	void Logout();

	// 并不是返回用户准确年龄，只是返回一个年龄区间，比如大于18岁，只会返回18；
	int GetCurrentUserAgeLimit();

	/// 获取用户剩余时长
	int GetCurrentUserRemainTime();

	void CheckPayLimit(int Amount,TFunction<void(bool Status)> CallBack,TFunction<void(const FString& Msg)> FailureHandler);

	/// 上报消费结果 
	/// - Parameter amount: 支付金额，单位分
	void SubmitPayResult(int Amount,
		TFunction<void(bool Success)> CallBack,
		TFunction<void(const FString& Msg)> FailureHandler);

private:
	static TSharedPtr<AAUImpl> Instance;

	FString CurrentUserID;

	TFunction<void(bool Result, const FString& Msg)> CurrentStartUpCallBack;

	void PerformStartUpCallBack(bool Result, const FString& Msg);

	TSharedPtr<bool> StandAloneEnable;
	
	TSharedPtr<AAUServer> Server;

	virtual void KickOut(AAUTimeBoundary Boundary, const FString& Title, const FString& Content) override;
	
	void Login(const FString& AccessToken, int AgeLimit, bool IsFirst = false);

	void ShowRealNameUI(UAAUManualRealNameWidget *Widget, AAURealNameWordType Type);

	void TryAgainStartUp(const FString& ErrMsg);

	void ShowHealthTipUI(const FString& Title, const FString& Content, int RemainTime, AAUHealthTipType Type);
};
