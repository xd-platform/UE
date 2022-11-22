// Fill out your copyright notice in the Description page of Project Settings.


#include "XDServiceWidgetAccount.h"

#include "TUDebuger.h"
#include "TUDeviceInfo.h"
#include "XDGAccount.h"
#include "XDGAccountBPLibrary.h"
#include "XDUE.h"
#include "XUType.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "SubWidgets/EditableTextContainerWidget.h"
#include "XDSDK/SubWidgets/ServiceItemWidget.h"
#include "XDSDK/XDSDK.h"

void UXDServiceWidgetAccount::OnLoginClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
    
	TArray<FString> Arr;
	Arr.Add("TAPTAP");
	Arr.Add("APPLE");
	Arr.Add("GOOGLE");
	Arr.Add("FACEBOOK");
	Arr.Add("LINE");
	Arr.Add("TWITTER");
	Arr.Add("GUEST");

	UXDGAccountBPLibrary::Login(Arr);
#endif
}

void UXDServiceWidgetAccount::OnAutoLoginClicked() {
#if PLATFORM_IOS || PLATFORM_ANDROID
	const FString LoginTypeStr = "Default";
	UXDGAccountBPLibrary::LoginByType(LoginTypeStr);
#elif PLATFORM_WINDOWS || PLATFORM_MAC
	auto Success = [](FXUUser User)
	{
		TUDebuger::DisplayShow(FString::Printf(TEXT("LoginSuccess：%s\r\n"), *TUJsonHelper::GetJsonString(User)));
	};

	auto Failed = [](FXUError Error)
	{
		DEMO_LOG(TEXT("登录失败：\r\n%s \r\n%s"), *TUJsonHelper::GetJsonString(Error), *TUJsonHelper::GetJsonString(Error.ExtraData));
		if (Error.code == 40021 && Error.ExtraData.IsValid())
		{
			FString Platform = Error.ExtraData->GetStringField("loginType");
			FString Email = Error.ExtraData->GetStringField("email");
			DEMO_LOG(TEXT("当前 %s 账号所关联的邮箱 %s 未被验证，请前往 %s 验证邮箱后重新登录游戏"), *Platform, *Email, *Platform);
		}
		else if (Error.code == 40902 && Error.ExtraData.IsValid())
		{
			FString Platform = Error.ExtraData->GetStringField("loginType");
			FString Email = Error.ExtraData->GetStringField("email");
			auto Conflicts = Error.ExtraData->GetArrayField("conflicts");
			FString Content = FString::Printf(TEXT("当前 %s 账号所关联的邮箱 %s 对应的游戏账号已绑定"), *Platform, *Email);
			TArray<FString> Accounts;
			for (auto JsonValue : Conflicts)
			{
				Accounts.Add(JsonValue->AsObject()->GetStringField("loginType"));
			}
			Content += FString::Join(Accounts, TEXT("、"));
			Content += TEXT("，请使用该邮箱所关联的其他平台游戏账号登录后进入「账号安全中心」手动进行账号绑定、解绑操作。");
			DEMO_LOG(TEXT("%s"), *Content);
		}
	};
	XDUE::LoginByType(XUType::LoginType::Default, Success, Failed);
#endif
}

void UXDServiceWidgetAccount::OnConsoleLoginClicked() {
#if PLATFORM_IOS || PLATFORM_ANDROID

#elif PLATFORM_WINDOWS || PLATFORM_MAC
	XDUE::LoginByConsole([](const FXUUser& User) {
		TUDebuger::DisplayShow(TEXT("登录成功: ") + TUJsonHelper::GetJsonString(User));
	}, []() {
		TUDebuger::WarningShow(TEXT("登录失败，未绑定XDUser"));
	}, [](const FXUError& Error) {
		TUDebuger::WarningShow(TEXT("发生错误，错误信息") + TUJsonHelper::GetJsonString(Error));
	});
#endif
}

void UXDServiceWidgetAccount::OnDeviceIDClicked() {
	TUDebuger::DisplayShow(TUDeviceInfo::GetLoginId());
}

void UXDServiceWidgetAccount::OnLoginByTypeClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	const FString LoginTypeStr = CB_LoginByType_LoginType->GetSelectedOption();
	UXDGAccountBPLibrary::LoginByType(LoginTypeStr);
#elif PLATFORM_WINDOWS || PLATFORM_MAC
	auto Success = [](FXUUser User)
	{
		TUDebuger::DisplayShow(FString::Printf(TEXT("LoginSuccess：%s\r\n"), *TUJsonHelper::GetJsonString(User)));
	};

	auto Failed = [](FXUError Error)
	{
		DEMO_LOG(TEXT("登录失败：\r\n%s \r\n%s"), *TUJsonHelper::GetJsonString(Error), *TUJsonHelper::GetJsonString(Error.ExtraData));
		if (Error.code == 40021 && Error.ExtraData.IsValid())
		{
			FString Platform = Error.ExtraData->GetStringField("loginType");
			FString Email = Error.ExtraData->GetStringField("email");
			DEMO_LOG(TEXT("当前 %s 账号所关联的邮箱 %s 未被验证，请前往 %s 验证邮箱后重新登录游戏"), *Platform, *Email, *Platform);
		}
		else if (Error.code == 40902 && Error.ExtraData.IsValid())
		{
			FString Platform = Error.ExtraData->GetStringField("loginType");
			FString Email = Error.ExtraData->GetStringField("email");
			auto Conflicts = Error.ExtraData->GetArrayField("conflicts");
			FString Content = FString::Printf(TEXT("当前 %s 账号所关联的邮箱 %s 对应的游戏账号已绑定"), *Platform, *Email);
			TArray<FString> Accounts;
			for (auto JsonValue : Conflicts)
			{
				Accounts.Add(JsonValue->AsObject()->GetStringField("loginType"));
			}
			Content += FString::Join(Accounts, TEXT("、"));
			Content += TEXT("，请使用该邮箱所关联的其他平台游戏账号登录后进入「账号安全中心」手动进行账号绑定、解绑操作。");
			DEMO_LOG(TEXT("%s"), *Content);
		}
	};
	const int32 Index = CB_LoginByType_LoginType->GetSelectedIndex();
	ETempDemoLoginType TempType = static_cast<ETempDemoLoginType>(StaticEnum<ETempDemoLoginType>()->GetValueByIndex(Index));
	XDUE::LoginByType(TempType == ETempDemoLoginType::Default ? XUType::LoginType::Default : static_cast<XUType::LoginType>(TempType), Success, Failed);
#endif
}

void UXDServiceWidgetAccount::OnAddUserStatusChangeCallbackClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	UXDGAccountBPLibrary::AddUserStatusChangeCallback();
#elif PLATFORM_WINDOWS || PLATFORM_MAC
	
#endif
}

void UXDServiceWidgetAccount::OnGetUserClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	UXDGAccountBPLibrary::GetUser();
#elif PLATFORM_WINDOWS || PLATFORM_MAC
	if (TSharedPtr<FXUUser> User = XDUE::GetUserInfo())
	{
		DEMO_LOG(TEXT("GetUser success. %s"), *TUJsonHelper::GetJsonString(User));
	}
	else
	{
		DEMO_LOG(TEXT("GetUser failed. Plase login."));
	}
#endif
}

void UXDServiceWidgetAccount::OnOpenUserCenterClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	UXDGAccountBPLibrary::OpenUserCenter();
#elif PLATFORM_WINDOWS || PLATFORM_MAC
	XDUE::OpenUserCenter();
#endif
}

void UXDServiceWidgetAccount::OnLogoutClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	UXDGAccountBPLibrary::Logout();
#elif PLATFORM_WINDOWS || PLATFORM_MAC
	XDUE::Logout();
#endif
	DEMO_LOG(TEXT("Logout."));
}

void UXDServiceWidgetAccount::OnOpenAccountCancellationClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	UXDGAccountBPLibrary::OpenAccountCancellation();
#elif PLATFORM_WINDOWS || PLATFORM_MAC
	XDUE::AccountCancellation();
#endif
}

void UXDServiceWidgetAccount::OnIsTokenActiveWithTypeClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	const FString LoginType = ETB_Token_LoginType->GetText().ToString();
	UXDGAccountBPLibrary::IsTokenActiveWithType(LoginType);
#endif
}

void UXDServiceWidgetAccount::OnBindByTypeClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	const FString LoginType = CB_Bind_LoginType->GetSelectedOption();
	UXDGAccountBPLibrary::BindByType(LoginType);
#elif PLATFORM_WINDOWS || PLATFORM_MAC

	const int32 Index = CB_Bind_LoginType->GetSelectedIndex();
	ETempDemoLoginType TempType = static_cast<ETempDemoLoginType>(StaticEnum<ETempDemoLoginType>()->GetValueByIndex(Index));
	auto LoginType = TempType == ETempDemoLoginType::Default ? XUType::LoginType::Default : static_cast<XUType::LoginType>(TempType);
	XDUE::BindByType(LoginType, [](bool Success, const FXUError& Error) {
		if (Success) {
			TUDebuger::DisplayShow(TEXT("绑定成功"));
		} else {
			TUDebuger::WarningShow(TEXT("绑定失败：") + Error.msg);
		}
	});
#endif
}

void UXDServiceWidgetAccount::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UEnum* EnumClass = StaticEnum<ETempDemoLoginType>();
	for (int32 i = 0; i < EnumClass->NumEnums() - 1; ++i)
	{
		const FString OptionStr = EnumClass->GetNameStringByIndex(i);
		CB_LoginByType_LoginType->AddOption(OptionStr);
		CB_Bind_LoginType->AddOption(OptionStr);
	}
	CB_LoginByType_LoginType->SetSelectedIndex(0);
	CB_Bind_LoginType->SetSelectedIndex(0);

	
	Login->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetAccount::OnLoginClicked);
	LoginByType->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetAccount::OnLoginByTypeClicked);
	AddUserStatusChangeCallback->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetAccount::OnAddUserStatusChangeCallbackClicked);
	GetUser->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetAccount::OnGetUserClicked);
	OpenUserCenter->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetAccount::OnOpenUserCenterClicked);
	Logout->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetAccount::OnLogoutClicked);
	OpenAccountCancellation->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetAccount::OnOpenAccountCancellationClicked);
	IsTokenActiveWithType->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetAccount::OnIsTokenActiveWithTypeClicked);
	BindByType->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetAccount::OnBindByTypeClicked);
	DeviceID->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetAccount::OnDeviceIDClicked);
	AutoLogin->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetAccount::OnAutoLoginClicked);
	ConsoleLogin->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetAccount::OnConsoleLoginClicked);
	
	XDUE::OnUserStatusChange.AddLambda([](XUType::UserChangeState UserState, const FString& Msg) {
		if (UserState == XUType::UserLogout) {
			TUDebuger::DisplayShow(TEXT("游戏账号应登出"));
		} else if (UserState == XUType::UserBindSuccess) {
			TUDebuger::DisplayShow(Msg + TEXT("账号被绑定"));
		} else if (UserState == XUType::UserUnBindSuccess) {
			TUDebuger::DisplayShow(Msg + TEXT("账号被解绑"));
		}
	});


#if PLATFORM_IOS || PLATFORM_ANDROID
	FXDGAccountModule::OnXDGSDKLoginSucceed.AddUObject(this, &UXDServiceWidgetAccount::OnXDGSDKLoginSucceed);
	FXDGAccountModule::OnXDGSDKLoginFailed.AddUObject(this, &UXDServiceWidgetAccount::OnXDGSDKLoginFailed);
	FXDGAccountModule::OnXDGSDKGetUserSucceed.AddUObject(this, &UXDServiceWidgetAccount::OnXDGSDKGetUserSucceed);
	FXDGAccountModule::OnXDGSDKGetUserFailed.AddUObject(this, &UXDServiceWidgetAccount::OnXDGSDKGetUserFailed);
	FXDGAccountModule::OnXDGSDKUserStateChanged.AddUObject(this, &UXDServiceWidgetAccount::OnXDGSDKUserStateChanged);
	FXDGAccountModule::OnXDGSDKBindByTypeCompleted.AddUObject(this, &UXDServiceWidgetAccount::OnXDGSDKBindByTypeCompleted);
#endif
}

void UXDServiceWidgetAccount::OnXDGSDKLoginSucceed(const FString& userJson)
{
	DEMO_LOG(TEXT("OnXDGSDKLoginSucceed: %s"), *userJson);
}

void UXDServiceWidgetAccount::OnXDGSDKLoginFailed(const int32 code, const FString& msg, const FString& data)
{
	DEMO_LOG(TEXT("OnXDGSDKLoginFailed: code-%d msg-%s data-%s"), code, *msg, *data);
}

void UXDServiceWidgetAccount::OnXDGSDKGetUserSucceed(const FString& userJson)
{
	DEMO_LOG(TEXT("OnXDGSDKGetUserSucceed: %s"), *userJson);
}

void UXDServiceWidgetAccount::OnXDGSDKGetUserFailed(const int32 code, const FString& msg)
{
	DEMO_LOG(TEXT("OnXDGSDKGetUserFailed: code-%d msg-%s"), code, *msg);
}

void UXDServiceWidgetAccount::OnXDGSDKUserStateChanged(const int32 code, const FString& msg)
{
	DEMO_LOG(TEXT("OnXDGSDKUserStateChanged: code-%d msg-%s"), code, *msg);
}

void UXDServiceWidgetAccount::OnXDGSDKBindByTypeCompleted(const bool success, const int32 code, const FString& msg)
{
	DEMO_LOG(TEXT("OnXDGSDKBindByTypeCompleted: success-%s code-%d msg-%s"), success ? TEXT("True") : TEXT("False"), code, *msg);
}
