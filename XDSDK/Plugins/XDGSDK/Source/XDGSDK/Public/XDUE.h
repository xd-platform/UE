#pragma once
#include "XUError.h"
#include "XUIpInfoModel.h"
#include "XUUser.h"
#include "XUType.h"

class XDGSDK_API XDUE {
public:

	DECLARE_MULTICAST_DELEGATE(XUSimpleDelegate)

	// 配置文件初始化 ***UXDGCommonBPLibrary::InitSDK
	static void InitSDK(const FString& GameVersion, TFunction<void(bool Result, const FString& Message)> CallBack);

	// 手动初始化 ***UXDGCommonBPLibrary::InitSDK
	static void InitSDK(const XUType::Config& Config, TFunction<void(bool Result, const FString& Message)> CallBack);

	// 登录接口***UXDGAccountBPLibrary::LoginByType
	static void LoginByType(XUType::LoginType Type, TFunction<void(const FXUUser& User)> SuccessBlock, TFunction<void(const FXUError& Error)> FailBlock);

	// 使用前判断下IsValid，登录成功后才有值，也可以判断用户是否是已登录的状态。***UXDGAccountBPLibrary::GetUser
	static TSharedPtr<FXUUser> GetUserInfo();
	//！删除
	static TSharedPtr<FXUTokenModel> GetAccessToken();

	// 使用前判断下IsValid，如果为空，那么使用异步方法请求网络IP ***UXDGCommonBPLibrary::GetRegionInfo
	static TSharedPtr<FXUIpInfoModel> GetIPInfo();
	static void GetIPInfo(TFunction<void(TSharedPtr<FXUIpInfoModel> IpInfo)> CallBack);//Remote

	// 是否已经初始化 ***UXDGCommonBPLibrary::IsInitialized
	static bool IsInitialized();

	// 设置语言 ***UXDGCommonBPLibrary::SetLanguage @TODO 对应枚举值
	static void SetLanguage(XUType::LangType Type);

	// 登出 ***UXDGAccountBPLibrary::Logout
	static void Logout();

	// 注销账户 ***UXDGAccountBPLibrary::OpenAccountCancellation
	static void AccountCancellation();

	// 被动登出的回调，游戏方需要监听这个代理委托，触发游戏的登出操作（目前只有注销账户才会触发这个） @TODO 注销之后的回调
	static XUSimpleDelegate OnLogout;

	// 打开用户中心，有绑定和解绑的回调，如果Error指针为空，说明绑定或解绑成功 ***UXDGAccountBPLibrary::OpenUserCenter
	static void OpenUserCenter(TFunction<void(XUType::LoginType Type, TSharedPtr<FXUError>)> BindCallBack,
	TFunction<void(XUType::LoginType Type, TSharedPtr<FXUError>)> UnbindCallBack);

	// 检测是否需要补款，如果发现需要补款，会有弹窗封住画面，让用户去移动端完成补款（某些游戏需求） ***UXDGPaymentBPLibrary::CheckRefundStatusWithUI
	static void CheckPay(TFunction<void(XUType::CheckPayType CheckType)> SuccessBlock, TFunction<void(const FXUError& Error)> FailBlock);

	// 打开客服中心 ***UXDGCommonBPLibrary::Report
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
	* @param CallBack		 支付结果回调(仅支持国内支付) ***UXDGPaymentBPLibrary::PayWithWeb
	*/ 
	static void OpenWebPay(const FString& ServerId,
	                       const FString& RoleId,
	                       const FString& ProductId,
	                       const FString& OrderId,
	                       const FString& ProductName = "",
	                       float PayAmount = 0,
	                       const FString& Ext = "",
	                       TFunction<void(XUType::PayResult Result)> CallBack = nullptr);

	// 用户是否同意推送服务（韩国专属） ***UXDGCommonBPLibrary::SetCurrentUserPushServiceEnable
	static void SetPushServiceEnable(bool enable);
	static bool IsPushServiceEnable(); //***UXDGCommonBPLibrary::IsCurrentUserPushServiceEnable


#if !UE_BUILD_SHIPPING
	// only test

	static void Test();
	//***@TODO
	static void ResetPrivacy();
#endif
	
};
