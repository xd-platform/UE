#include "XDUE.h"

#include "TapUELogin.h"
#include "TUDebuger.h"
#include "XUConfigManager.h"
#include "XUImpl.h"
#include "XUThirdAuthHelper.h"
#include "XUSettings.h"
#include "XDGSDK/UI/XUUserCenterWidget.h"
#include "XDGSDK/UI/XUPayHintAlert.h"

XDUE::XUSimpleDelegate XDUE::OnLogout;

void XDUE::InitSDK(const FString& GameVersion, TFunction<void(bool Result, const FString& Message)> CallBack) {
	if (IsInitialized()) {
		if (CallBack) {
			CallBack(true, TEXT("已经初始化"));
		}
		return;
	}
	XUImpl::Get()->InitSDK(GameVersion, CallBack);
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
		Logout();
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
	// await TDSUser.Logout();
	TapUELogin::Logout();
	FXUUser::ClearUserData();
}

void XDUE::AccountCancellation() {
	if (!FXUUser::GetLocalModel().IsValid()) {
		TUDebuger::WarningLog("Please Login First");
		return;
	}
	XUImpl::Get()->AccountCancellation();
}

void XDUE::OpenUserCenter(TFunction<void(XUType::LoginType Type, TSharedPtr<FXUError>)> BindCallBack,
                          TFunction<void(XUType::LoginType Type, TSharedPtr<FXUError>)> UnbindCallBack) {
	if (!FXUUser::GetLocalModel().IsValid()) {
		TUDebuger::WarningLog("Please Login First");
		return;
	}

	UXUUserCenterWidget::ShowWidget(BindCallBack, UnbindCallBack);
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
		FPlatformProcess::LaunchURL(*UrlStr, nullptr, nullptr);
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


