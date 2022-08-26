#pragma once
#include "AAUType.h"

class ANTIADDICTION_API AntiAddictionUE {
public:

	DECLARE_MULTICAST_DELEGATE(SimpleDelegate)

	// 未成年可玩时间耗尽，强制退出游戏，可以做一些保存用户数据的行为
	static SimpleDelegate OnExit;

	// 未成年可玩时间耗尽，可以选择切换账号（配置支持），游戏可以触发退出账号的逻辑
	static SimpleDelegate OnSwitchAccount;

	// 防沉迷初始化
	static void Init(const AAUType::Config& Config);

	// 启动防沉迷
	static void Login(const FString& UserID, TFunction<void(bool Result, const FString& Msg)> CallBack);

	// 防沉迷退出
	static void Logout();

	// 进入游戏，游戏从后台激活时调用该API
	static void EnterGame();

	// 离开游戏，游戏进入后台时调用
	static void LeaveGame();

	// 并不是返回用户准确年龄，只是返回一个年龄区间，比如大于18岁，只会返回18；
	static int GetCurrentUserAgeLimit();

	/// 获取用户剩余时长
	static int GetCurrentUserRemainTime();
	

	/// 查询能否支付
	/// - Parameter amount: 支付金额，单位分
	/// - Parameter status: true:可以付费 false:限制消费
	static void CheckPayLimit(int Amount,
		TFunction<void(bool Status)> CallBack,
		TFunction<void(const FString& Msg)> FailureHandler);

	/// 上报消费结果 
	/// - Parameter amount: 支付金额，单位分
	static void SubmitPayResult(int Amount,
		TFunction<void(bool Success)> CallBack,
		TFunction<void(const FString& Msg)> FailureHandler);



	static void Test();
	
};

