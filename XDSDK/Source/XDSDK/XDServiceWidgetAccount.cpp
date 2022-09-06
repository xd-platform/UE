// Fill out your copyright notice in the Description page of Project Settings.


#include "XDServiceWidgetAccount.h"

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
	const TArray<FString> Arr =ET_Login_LoginTypes->MakeStringArray();
	UXDGAccountBPLibrary::Login(Arr);
#endif
}

void UXDServiceWidgetAccount::OnLoginByTypeClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	const FString LoginTypeStr = CB_LoginByType_LoginType->GetSelectedOption();
	UXDGAccountBPLibrary::LoginByType(LoginTypeStr);
#elif PLATFORM_WINDOWS || PLATFORM_MAC
	auto Success = [](FXUUser User)
	{
		DEMO_LOG(TEXT("LoginSuccess：\r\n"), *TUJsonHelper::GetJsonString(User));
	};

	auto Failed = [](FXUError Error)
	{
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
		else
		{
			DEMO_LOG(TEXT("登录失败：\r\n%s \r\n%s"), *TUJsonHelper::GetJsonString(Error), *TUJsonHelper::GetJsonString(Error.ExtraData));
		};
	};
	const int32 Index = CB_LoginByType_LoginType->GetSelectedIndex();
	XUType::LoginType Type = static_cast<XUType::LoginType>(StaticEnum<ETempDemoLoginType>()->GetValueByIndex(Index));
	XDUE::LoginByType(Type, Success, Failed);
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
	auto Success = [](XUType::LoginType Type, TSharedPtr<FXUError> Error)
	{
		if (Error.IsValid())
		{
			DEMO_LOG(TEXT("绑定失败, Error: %s"), *Error->msg);
		}
		else
		{
			DEMO_LOG(TEXT("绑定成功, Type: %d"), Type);
		}
	};

	auto Failed = [](XUType::LoginType Type, TSharedPtr<FXUError> Error)
	{
		if (Error.IsValid())
		{
			DEMO_LOG(TEXT("解绑失败, Error: %s"), *Error->msg);
		}
		else
		{
			DEMO_LOG(TEXT("解绑成功, Type: %d"), Type);
		}
	};
	XDUE::OpenUserCenter(Success, Failed);
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
	const FString LoginType = ETB_Bind_LoginType->GetText().ToString();
	UXDGAccountBPLibrary::BindByType(LoginType);
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
	}
	CB_LoginByType_LoginType->SetSelectedIndex(0);
	
	Login->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetAccount::OnLoginClicked);
	LoginByType->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetAccount::OnLoginByTypeClicked);
	AddUserStatusChangeCallback->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetAccount::OnAddUserStatusChangeCallbackClicked);
	GetUser->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetAccount::OnGetUserClicked);
	OpenUserCenter->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetAccount::OnOpenUserCenterClicked);
	Logout->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetAccount::OnLogoutClicked);
	OpenAccountCancellation->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetAccount::OnOpenAccountCancellationClicked);
	IsTokenActiveWithType->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetAccount::OnIsTokenActiveWithTypeClicked);
	BindByType->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetAccount::OnBindByTypeClicked);

#if PLATFORM_IOS || PLATFORM_ANDROID
	FXDGAccountModule::OnXDGSDKLoginSucceed.AddUObject(this, &UServiceWidgetAccount::OnXDGSDKLoginSucceed);
	FXDGAccountModule::OnXDGSDKLoginFailed.AddUObject(this, &UServiceWidgetAccount::OnXDGSDKLoginFailed);
	FXDGAccountModule::OnXDGSDKGetUserSucceed.AddUObject(this, &UServiceWidgetAccount::OnXDGSDKGetUserSucceed);
	FXDGAccountModule::OnXDGSDKGetUserFailed.AddUObject(this, &UServiceWidgetAccount::OnXDGSDKGetUserFailed);
	FXDGAccountModule::OnXDGSDKUserStateChanged.AddUObject(this, &UServiceWidgetAccount::OnXDGSDKUserStateChanged);
	FXDGAccountModule::OnXDGSDKBindByTypeCompleted.AddUObject(this, &UServiceWidgetAccount::OnXDGSDKBindByTypeCompleted);
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
