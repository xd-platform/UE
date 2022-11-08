#include "XDUE.h"
#include "TapCommonBPLibrary.h"
#include "TapUEDB.h"
#include "TapUELogin.h"
#include "TUDebuger.h"
#include "TUHelper.h"
#include "XUConfigManager.h"
#include "XUImpl.h"
#include "XUThirdAuthHelper.h"
#include "XUSettings.h"
#include "XDGSDK/UI/XUUserCenterWidget.h"
#include "XDGSDK/UI/XUPayHintAlert.h"

XDUE::FUserStateChangeDelegate XDUE::OnUserStatusChange;

void XDUE::InitSDK(TFunction<void(bool Result, const FString& Message)> CallBack, TFunction<void(TSharedRef<XUType::Config> Config)> EditConfig) {
	if (IsInitialized()) {
		if (CallBack) {
			CallBack(true, TEXT("已经初始化"));
		}
		return;
	}
	XUImpl::Get()->InitSDK(CallBack, EditConfig);
}

void XDUE::InitSDK(const XUType::Config& Config, TFunction<void(bool Result, const FString& Message)> CallBack) {
	if (IsInitialized()) {
		if (CallBack) {
			CallBack(true, TEXT("已经初始化"));
		}
		return;
	}
	TSharedPtr<XUType::Config> ConfigPtr = MakeShareable(new XUType::Config);
	XUImpl::Get()->InitSDK(ConfigPtr, CallBack);
}

void XDUE::LoginByType(XUType::LoginType Type, TFunction<void(const FXUUser& User)> SuccessBlock,
	TFunction<void(const FXUError& Error)> FailBlock) {
	if (!IsInitialized())
	{
		if (FailBlock)
		{
			FailBlock(FXUError(FString::Printf(TEXT("Please Init First Before Call %s"), ANSI_TO_TCHAR(__FUNCTION__))));
		}
		return;
	}

	XUImpl::Get()->LoginByType(Type,
	[=](TSharedPtr<FXUUser> user)
	{
		if (SuccessBlock)
		{
			SuccessBlock(*user.Get());
		}
	},
	[=](FXUError error)
	{
		if (FailBlock)
		{
			FailBlock(error);
		}
	});
}

TSharedPtr<FXUUser> XDUE::GetUserInfo() {
	auto UserPtr = FXUUser::GetLocalModel();
	// 判断状态是否一致，如果不一致，那么退出登录，重新登录
	if (UserPtr.IsValid() == FXUTokenModel::GetLocalModel().IsValid()) {
		return UserPtr;
	}
	else {
		FXUUser::ClearUserData();
		return nullptr;
	}
}

TSharedPtr<FXUTokenModel> XDUE::GetAccessToken() {
	return FXUTokenModel::GetLocalModel();
}

TSharedPtr<FXUIpInfoModel> XDUE::GetIPInfo() {
	return FXUIpInfoModel::GetLocalModel();
}

void XDUE::GetIPInfo(TFunction<void(TSharedPtr<FXUIpInfoModel> IpInfo)> CallBack) {
	XUConfigManager::GetRegionInfo(CallBack);
}

bool XDUE::IsInitialized() {
	return XUConfigManager::IsGameInited();
}

void XDUE::SetLanguage(XUType::LangType Type) {
	XUSettings::SetLanguage(Type);
}

void XDUE::Logout() {
	XUImpl::Get()->Logout();
}

void XDUE::AccountCancellation() {
	if (!FXUUser::GetLocalModel().IsValid()) {
		TUDebuger::WarningLog("Please Login First");
		return;
	}
	XUImpl::Get()->AccountCancellation();
}

void XDUE::OpenUserCenter() {
	if (!FXUUser::GetLocalModel().IsValid()) {
		TUDebuger::WarningLog("Please Login First");
		return;
	}

	UXUUserCenterWidget::ShowWidget();
}

void XDUE::CheckPay(TFunction<void(XUType::CheckPayType CheckType)> SuccessBlock,
	TFunction<void(const FXUError& Error)> FailBlock) {
	if (!FXUUser::GetLocalModel().IsValid()) {
		if (FailBlock)
		{
			FailBlock(FXUError("Please Login First"));
		}
		return;
	}
	XUImpl::Get()->CheckPay([=](XUType::CheckPayType CheckType)
	{
		if (CheckType != XUType::None)
		{
			UXUPayHintAlert::Show(CheckType);
		}
		if (SuccessBlock)
		{
			SuccessBlock(CheckType);
		}
	}, FailBlock);
}

void XDUE::OpenCustomerCenter(const FString& ServerId, const FString& RoleId, const FString& RoleName) {
	FString UrlStr = XUImpl::Get()->GetCustomerCenter(ServerId, RoleId, RoleName);

	if (UrlStr.IsEmpty()) {
		TUDebuger::ErrorLog("please login first");
	} else {
		TUHelper::LaunchURL(*UrlStr, nullptr, nullptr);
	}
}

void XDUE::OpenWebPay(const FString& ServerId, const FString& RoleId, const FString& ProductId, const FString& OrderId,
	const FString& ProductName, float PayAmount, const FString& Ext,
	TFunction<void(XUType::PayResult Result)> CallBack) {
	XUImpl::Get()->OpenWebPay(ServerId, RoleId, ProductId, OrderId, ProductName, PayAmount, Ext, CallBack);
}


void XDUE::SetPushServiceEnable(bool enable) {
	FXUUser::SetPushServiceEnable(enable);
}

bool XDUE::IsPushServiceEnable() {
	return FXUUser::IsPushServiceEnable();
}

void XDUE::TrackUser()
{
	auto UserPtr = GetUserInfo();
	if (UserPtr.IsValid()) {
		TrackUser(UserPtr->userId);
	}
}

//TapDB 统计用户
void XDUE::TrackUser(FString userId)
{
	TapUEDB::SetUserWithLoginType(userId, "");
}

//TapDB 设置属性
void XDUE::TrackRole(FString serverId, FString roleId, FString roleName, int32 level)
{
	//目前移动SDK也没有使用这个 roleId 
	TapUEDB::SetLevel(level);
	TapUEDB::SetName(roleName);
	TapUEDB::SetServer(serverId);
}

//TapDB 事件埋点
void XDUE::TrackEvent(FString eventName)
{
	TapUEDB::TrackEvent(eventName, nullptr);
}

//TapDB 成就埋点
void XDUE::TrackAchievement()
{
	//没有tap db 只有第三方
}

//TapDB 完成新手引导埋点
void XDUE::EventCompletedTutorial()
{
	//没有tap db 只有第三方
}

//TapDB 创建角色埋点
void XDUE::EventCreateRole() {
	//没有tap db 只有第三方
}

void XDUE::BindByType(XUType::LoginType BindType, TFunction<void(bool Success, const FXUError& Error)> CallBack) {
	XUImpl::Get()->BindByType(BindType, CallBack);
}

#if !UE_BUILD_SHIPPING
// only test

void XDUE::Test() {
	// enum class ENetworkConnectionType : uint8
	// {
	// 	/**
	// 	 * Enumerates the network connection types
	// 	 */
	// 	Unknown,
	// 	None,
	// 	AirplaneMode,
	// 	Cell,
	// 	WiFi,
	// 	WiMAX,
	// 	Bluetooth,
	// 	Ethernet,
	// };
	TUDebuger::DisplayShow(FString::Printf(TEXT("GetNetworkConnectionType: %d"), FPlatformMisc::GetNetworkConnectionType()));
}

void XDUE::ResetPrivacy() {
	XUImpl::Get()->ResetPrivacy();
}

#endif


