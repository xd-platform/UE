#pragma once
#include "XUError.h"
#include "XUIpInfoModel.h"
#include "XUUser.h"
#include "XUType.h"

class XDGSDK_API XDUE {
public:

	DECLARE_MULTICAST_DELEGATE(XUSimpleDelegate)

	// 配置文件初始化
	static void InitSDK(const FString& GameVersion, TFunction<void(bool Result, const FString& Message)> CallBack);

	// 手动初始化
	static void InitSDK(const XUType::Config& Config, TFunction<void(bool Result, const FString& Message)> CallBack);

	// 登录接口
	static void LoginByType(XUType::LoginType Type, TFunction<void(const FXUUser& User)> SuccessBlock, TFunction<void(const FXUError& Error)> FailBlock);

	// 使用前判断下IsValid，登录成功后才有值，也可以判断用户是否是已登录的状态。
	static TSharedPtr<FXUUser> GetUserInfo();
	
	static TSharedPtr<FXUTokenModel> GetAccessToken();

	// 使用前判断下IsValid，如果为空，那么使用异步方法请求网络IP
	static TSharedPtr<FXUIpInfoModel> GetIPInfo();
	static void GetIPInfo(TFunction<void(TSharedPtr<FXUIpInfoModel> IpInfo)> CallBack);

	// 是否已经初始化
	static bool IsInitialized();

	// 设置语言
	static void SetLanguage(XUType::LangType Type);

	// 登出
	static void Logout();

	// 注销账户
	static void AccountCancellation();

	// 被动登出的回调，游戏方需要监听这个代理委托，触发游戏的登出操作（目前只有注销账户才会触发这个）
	static XUSimpleDelegate OnLogout;

	// 打开用户中心，有绑定和解绑的回调，如果Error指针为空，说明绑定或解绑成功
	static void OpenUserCenter(TFunction<void(XUType::LoginType Type, TSharedPtr<FXUError>)> BindCallBack,
	TFunction<void(XUType::LoginType Type, TSharedPtr<FXUError>)> UnbindCallBack);

	// 检测是否需要补款，如果发现需要补款，会有弹窗封住画面，让用户去移动端完成补款（某些游戏需求）
	static void CheckPay(TFunction<void(XUType::CheckPayType CheckType)> SuccessBlock, TFunction<void(const FXUError& Error)> FailBlock);

	// 打开客服中心
	static void OpenCustomerCenter(const FString& ServerId, const FString& RoleId, const FString& RoleName);

	/**
	* 国外网页支付，国内扫码支付
	*
	* @param ServerId        服务器ID。所在服务器ID，不能有特殊字符，服务端支付回调会包含该字段，必填
	* @param RoleId          角色ID。支付角色ID，服务端支付回调会包含该字段，必填
	* @param ProductId       商品ID。游戏的商品ID（同ProductSkuCode），必填
	* @param OrderId         订单ID。游戏侧订单号，服务端支付回调会包含该字段，可选。
	* @param ProductName     商品名称。用于内嵌支付内部显示，可选
	* @param PayAmount       商品价格，可选
	* @param Ext			 附加信息。服务端支付回调会包含该字段，可选
	* @param CallBack		 支付结果回调(仅支持国内支付)
	*/
	static void OpenWebPay(const FString& ServerId,
	                       const FString& RoleId,
	                       const FString& ProductId,
	                       const FString& OrderId,
	                       const FString& ProductName = "",
	                       float PayAmount = 0,
	                       const FString& Ext = "",
	                       TFunction<void(XUType::PayResult Result)> CallBack = nullptr);

	// 用户是否同意推送服务（韩国专属）
	static void SetPushServiceEnable(bool enable);
	static bool IsPushServiceEnable();

	//TapDB 统计用户
	static void TrackUser(FString userId);

	//TapDB 设置属性
	static void TrackRole(FString serverId, FString roleId, FString roleName, int32 level);

	//TapDB 事件埋点
	static void TrackEvent(FString eventName);

	//TapDB 成就埋点
	static void TrackAchievement();

	//TapDB 完成新手引导埋点
	static void EventCompletedTutorial();

	//TapDB 创建角色埋点
	static void EventCreateRole();

#if !UE_BUILD_SHIPPING
	// only test

	static void Test();

	static void ResetPrivacy();
#endif
	
};
