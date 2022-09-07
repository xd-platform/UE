// Fill out your copyright notice in the Description page of Project Settings.


#include "XDServiceWidgetCommon.h"

#include "TUDebuger.h"
#include "XDGCommon.h"
#include "XDSDK/SubWidgets/ServiceItemWidget.h"
#include "XDUE.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "XDSDK/XDSDK.h"
#include "XDGSDK.h"
#include "XUSettings.h"
#if PLATFORM_IOS || PLATFORM_ANDROID
#include "TapUEMoment.h"
#include "XDGCommonBPLibrary.h"
#endif

void UXDServiceWidgetCommon::OnGetSDKVersionNameClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	const FString Version = UXDGCommonBPLibrary::GetSDKVersionName();
	DEMO_LOG(TEXT("GetSDKVersionName:%s"), *Version);
#elif PLATFORM_WINDOWS || PLATFORM_MAC
	DEMO_LOG(TEXT(XDUESDK_VERSION));
#endif
}

void UXDServiceWidgetCommon::OnSetLanguageClicked()
{
	const int32 LanguageID = CB_Language->GetSelectedIndex();
#if PLATFORM_IOS || PLATFORM_ANDROID
	UXDGCommonBPLibrary::SetLanguage(LanguageID);
#elif PLATFORM_WINDOWS || PLATFORM_MAC
	XDUE::SetLanguage(static_cast<XUType::LangType>(LanguageID));
#endif
}

void UXDServiceWidgetCommon::OnInitSDKClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	UXDGCommonBPLibrary::InitSDK();
#elif PLATFORM_WINDOWS || PLATFORM_MAC
	auto Callback = [](bool Result, FString Message)
	{
		DEMO_LOG_STRING(Message);
	};
	XDUE::InitSDK(TEXT("1.2.3"), Callback);
#endif
}

void UXDServiceWidgetCommon::OnIsInitializedClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	const bool bIsInitialized = UXDGCommonBPLibrary::IsInitialized();
	DEMO_LOG(TEXT("IsInitialized:%s"), bIsInitialized ? TEXT("True") : TEXT("False"));
#elif PLATFORM_WINDOWS || PLATFORM_MAC
	const bool bIsInitialized = XDUE::IsInitialized();
	DEMO_LOG(TEXT("IsInitialized:%s"), bIsInitialized ? TEXT("True") : TEXT("False"));
#endif
}

void UXDServiceWidgetCommon::OnReportClicked()
{
	const FString ServerIDStr = ETB_Report_ServerID->GetText().ToString();
	const FString RoleIDStr = ETB_Report_RoleID->GetText().ToString();
	const FString RoleNameStr = ETB_Report_RoleName->GetText().ToString();
#if PLATFORM_IOS || PLATFORM_ANDROID
	UXDGCommonBPLibrary::Report(ServerIDStr, RoleIDStr, RoleNameStr);
#elif PLATFORM_WINDOWS || PLATFORM_MAC
	XDUE::OpenCustomerCenter(ServerIDStr, RoleIDStr, RoleNameStr);
#endif
}

void UXDServiceWidgetCommon::OnStoreReviewClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	UXDGCommonBPLibrary::StoreReview();
#endif	
}

void UXDServiceWidgetCommon::OnTrackUserClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	const FString UserIDStr = ETB_User_UserID->GetText().ToString();
	UXDGCommonBPLibrary::TrackUser(UserIDStr);
#endif	
}

void UXDServiceWidgetCommon::OnTrackRoleClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	const FString ServerIDStr = ETB_Role_ServerID->GetText().ToString();
	const FString RoleIDStr = ETB_Role_RoleID->GetText().ToString();
	const FString RoleNameStr = ETB_Role_RoleName->GetText().ToString();
	const int32 Level = FCString::Atoi(*ETB_Role_Level->GetText().ToString());
	UXDGCommonBPLibrary::TrackRole(ServerIDStr, RoleIDStr, RoleNameStr, Level);
#endif	
}

void UXDServiceWidgetCommon::OnTrackEventClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	const FString EventNameStr = ETB_Event_EventName->GetText().ToString();
	UXDGCommonBPLibrary::TrackEvent(EventNameStr);
#endif	
}

void UXDServiceWidgetCommon::OnSetCurrentUserPushServiceEnableClicked()
{
	const bool bEnable = CheckBox->GetCheckedState() == ECheckBoxState::Checked;
#if PLATFORM_IOS || PLATFORM_ANDROID
	UXDGCommonBPLibrary::SetCurrentUserPushServiceEnable(bEnable);
#elif PLATFORM_WINDOWS || PLATFORM_MAC
	XDUE::SetPushServiceEnable(bEnable);
#endif
}

void UXDServiceWidgetCommon::OnIsCurrentUserPushServiceEnableClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	const bool bEnable = UXDGCommonBPLibrary::IsCurrentUserPushServiceEnable();
	DEMO_LOG(TEXT("IsCurrentUserPushServiceEnable:%s"), bEnable ? TEXT("True") : TEXT("False"));
#elif PLATFORM_WINDOWS || PLATFORM_MAC
	const bool bEnable = XDUE::IsPushServiceEnable();
	DEMO_LOG(TEXT("IsCurrentUserPushServiceEnable:%s"), bEnable ? TEXT("True") : TEXT("False"));
#endif
}

void UXDServiceWidgetCommon::OnGetRegionInfoClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	UXDGCommonBPLibrary::GetRegionInfo();
#elif PLATFORM_WINDOWS || PLATFORM_MAC
	if (TSharedPtr<FXUIpInfoModel> Info = XDUE::GetIPInfo())
	{
		DEMO_LOG(TEXT("GetIPInfo\r\n city:%s\r\n country:%s\r\n country_code:%s\r\n latitude:%s\r\n longitude:%s\r\n src_ip:%s\r\n timeZone:%s")
		, *Info->city
		, *Info->country
		, *Info->country_code
		, *Info->latitude
		, *Info->longitude
		, *Info->src_ip
		, *Info->timeZone);
	}
	else
	{
		auto Callback = [](TSharedPtr<FXUIpInfoModel> IpInfo)
		{
			DEMO_LOG(TEXT("AsynGetIPInfo\r\n city:%s\r\n country:%s\r\n country_code:%s\r\n latitude:%s\r\n longitude:%s\r\n src_ip:%s\r\n timeZone:%s")
		, *IpInfo->city
		, *IpInfo->country
		, *IpInfo->country_code
		, *IpInfo->latitude
		, *IpInfo->longitude
		, *IpInfo->src_ip
		, *IpInfo->timeZone);
		};
		XDUE::GetIPInfo(Callback);
	}
#endif
}

void UXDServiceWidgetCommon::OnGetXDGInfoJsonClicked()
{
#if PLATFORM_ANDROID
	UXDGCommonBPLibrary::GetXDGInfoJson();
#endif
}

void UXDServiceWidgetCommon::OnDisableAgreementUIClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	UXDGCommonBPLibrary::DisableAgreementUI();
#endif
}

void UXDServiceWidgetCommon::OnSetCountryRegionClicked()
{
	const FString RegionStr = CB_Region_Region->GetSelectedOption();
#if PLATFORM_IOS || PLATFORM_ANDROID
	UXDGCommonBPLibrary::SetCountryRegion(RegionStr);
#elif PLATFORM_WINDOWS || PLATFORM_MAC
	XUSettings::SetTargetCountryOrRegion(RegionStr);
#endif
}

void UXDServiceWidgetCommon::OnDevelopInitClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	const ETempDemoEnvironmentType Evn = static_cast<ETempDemoEnvironmentType>(StaticEnum<ETempDemoEnvironmentType>()->GetValueByNameString(CB_Init_EnvironmentBox->GetSelectedOption()));
	const ETempDemoRegionType Region = static_cast<ETempDemoRegionType>(StaticEnum<ETempDemoRegionType>()->GetValueByNameString(CB_Init_RegionBox->GetSelectedOption()));
	int32 Num = 0;
	switch (Region)
	{
	case ETempDemoRegionType::CN:
		Num = 2 + (Evn == ETempDemoEnvironmentType::Default ? 0 : 1);
		break;
	case ETempDemoRegionType::Global:
		Num = Evn == ETempDemoEnvironmentType::Default ? 0 : 1;
		break;
	default: ;
	}
	UXDGCommonBPLibrary::DevelopInit(Num);
#elif PLATFORM_WINDOWS || PLATFORM_MAC
	UEnum* EnvEnum = StaticEnum<ETempDemoEnvironmentType>();
	UEnum* RegionEnum = StaticEnum<ETempDemoRegionType>();
	if (CB_Init_EnvironmentBox->GetSelectedOption() == EnvEnum->GetNameStringByValue(static_cast<int32>(ETempDemoEnvironmentType::RND)))
	{
		TUDebuger::IsTest = true;
		if (CB_Init_RegionBox->GetSelectedOption() == RegionEnum->GetNameStringByValue(static_cast<int32>(ETempDemoRegionType::CN)))
		{
			XUSettings::UpdateConfigFileName("XDConfig-cn-rnd.json");
		}
		else if (CB_Init_RegionBox->GetSelectedOption() == RegionEnum->GetNameStringByValue(static_cast<int32>(ETempDemoRegionType::Global)))
		{
			XUSettings::UpdateConfigFileName("XDConfig-rnd.json");
		}
	}
	else if (CB_Init_EnvironmentBox->GetSelectedOption() == EnvEnum->GetNameStringByValue(static_cast<int32>(ETempDemoEnvironmentType::Default)))
	{
		TUDebuger::IsTest = false;
		if (CB_Init_RegionBox->GetSelectedOption() == RegionEnum->GetNameStringByValue(static_cast<int32>(ETempDemoRegionType::CN)))
		{
			XUSettings::UpdateConfigFileName("XDConfig-cn.json");
		}
		else if (CB_Init_RegionBox->GetSelectedOption() == RegionEnum->GetNameStringByValue(static_cast<int32>(ETempDemoRegionType::Global)))
		{
			XUSettings::UpdateConfigFileName("XDConfig.json");
		}
	}
	auto Callback = [](bool Result, FString Message)
	{
		DEMO_LOG_STRING(Message);
	};
	XDUE::InitSDK(TEXT("1.2.3"), Callback);
#endif
}

void UXDServiceWidgetCommon::OnTrackAchievementClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	UXDGCommonBPLibrary::TrackAchievement();
#endif
}

void UXDServiceWidgetCommon::OnEventCompletedTutorialClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	UXDGCommonBPLibrary::EventCompletedTutorial();
#endif
}

void UXDServiceWidgetCommon::OnEventCreateRoleClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	UXDGCommonBPLibrary::EventCreateRole();
#endif
}

void UXDServiceWidgetCommon::OnResetPrivacyClicked()
{
#if !UE_BUILD_SHIPPING && (PLATFORM_WINDOWS || PLATFORM_MAC)
	XDUE::ResetPrivacy();
	DEMO_LOG(TEXT("ResetPrivacy"));
#endif
}

void UXDServiceWidgetCommon::OnOpenMomentClicked()
{
#if !UE_BUILD_SHIPPING && (PLATFORM_IOS || PLATFORM_ANDROID)
	TapUEMoment::Open(TUMomentType::Orientation::LANDSCAPE);
#endif
}

void UXDServiceWidgetCommon::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UEnum* CountryEnumClass = StaticEnum<ETempDemoCountryType>();
	for (int32 i = 0; i < CountryEnumClass->NumEnums() - 1; ++i)
	{
		const FString Str = CountryEnumClass->GetNameStringByIndex(i);
		CB_Region_Region->AddOption(Str);
	}
	CB_Region_Region->SetSelectedIndex(0);

	UEnum* EnvEnumClass = StaticEnum<ETempDemoEnvironmentType>();
	for (int32 i = 0; i < EnvEnumClass->NumEnums() - 1; ++i)
	{
		const FString Str = EnvEnumClass->GetNameStringByIndex(i);
		CB_Init_EnvironmentBox->AddOption(Str);
	}
	CB_Init_EnvironmentBox->SetSelectedIndex(0);
	
	UEnum* RegionEnumClass = StaticEnum<ETempDemoRegionType>();
	for (int32 i = 0; i < RegionEnumClass->NumEnums() - 1; ++i)
	{
		const FString Str = RegionEnumClass->GetNameStringByIndex(i);
		CB_Init_RegionBox->AddOption(Str);
	}
	CB_Init_RegionBox->SetSelectedIndex(0);

	GetSDKVersionName->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetCommon::OnGetSDKVersionNameClicked);
	SetLanguage->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetCommon::OnSetLanguageClicked);
	InitSDK->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetCommon::OnInitSDKClicked);
	IsInitialized->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetCommon::OnIsInitializedClicked);
	Report->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetCommon::OnReportClicked);
	StoreReview->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetCommon::OnStoreReviewClicked);
	TrackUser->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetCommon::OnTrackUserClicked);
	TrackRole->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetCommon::OnTrackRoleClicked);
	TrackEvent->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetCommon::OnTrackEventClicked);
	SetCurrentUserPushServiceEnable->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetCommon::OnSetCurrentUserPushServiceEnableClicked);
	IsCurrentUserPushServiceEnable->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetCommon::OnIsCurrentUserPushServiceEnableClicked);
	GetRegionInfo->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetCommon::OnGetRegionInfoClicked);
	GetXDGInfoJson->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetCommon::OnGetXDGInfoJsonClicked);
	DisableAgreementUI->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetCommon::OnDisableAgreementUIClicked);
	SetCountryRegion->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetCommon::OnSetCountryRegionClicked);
	DevelopInit->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetCommon::OnDevelopInitClicked);
	TrackAchievement->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetCommon::OnTrackAchievementClicked);
	EventCompletedTutorial->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetCommon::OnEventCompletedTutorialClicked);
	EventCreateRole->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetCommon::OnEventCreateRoleClicked);
	
	ResetPrivacy->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetCommon::OnResetPrivacyClicked);
	OpenMoment->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetCommon::OnOpenMomentClicked);

	CB_Init_EnvironmentBox->OnSelectionChanged.AddDynamic(this, &UXDServiceWidgetCommon::OnEnvironmentSelectChanged);

	TUDebuger::IsTest = false;
	
	TUDebuger::ReplaceHosts.Add("https://xdsdk-6.xd.cn", "https://test-xdsdk-6.xd.cn");
	TUDebuger::ReplaceHosts.Add("https://xdsdk-intnl-6.xd.com", "https://test-xdsdk-intnl-6.xd.com");
	TUDebuger::ReplaceHosts.Add("https://ecdn-xdsdk-intnl-6.xd.com", "https://test-xdsdk-intnl-6.xd.com");
	TUDebuger::ReplaceHosts.Add("https://event-tracking-cn.cn-beijing.log.aliyuncs.com/logstores/sdk6-prod/track", "https://event-tracking-cn.cn-beijing.log.aliyuncs.com/logstores/sdk6-test/track");
	TUDebuger::ReplaceHosts.Add("https://event-tracking-global.ap-southeast-1.log.aliyuncs.com/logstores/sdk6-prod/track", "https://event-tracking-global.ap-southeast-1.log.aliyuncs.com/logstores/sdk6-test/track");

	TUDebuger::ReplaceHosts.Add("https://login-xdsdk.xd.cn", "https://xd-website.oss-cn-beijing.aliyuncs.com/xd-order-sgp/v1.0-dev/test/index.html");
	TUDebuger::ReplaceHosts.Add("https://login-xdsdk.xd.com", "https://xd-website.oss-cn-beijing.aliyuncs.com/xd-order-sgp/v1.0-dev/test/index.html");

	XDUE::OnLogout.AddLambda([]() {
		TUDebuger::DisplayShow(TEXT("游戏账号应登出"));
	});

#if PLATFORM_IOS || PLATFORM_ANDROID
	FXDGCommonModule::OnXDGSDKInitCompleted.AddUObject(this, &UServiceWidgetCommon::OnXDGSDKInitCompleted);
	FXDGCommonModule::OnXDGSDKShareCompleted.AddUObject(this, &UServiceWidgetCommon::OnXDGSDKShareCompleted);
	FXDGCommonModule::OnXDGSDKGetRegionInfoCompleted.AddUObject(this, &UServiceWidgetCommon::OnXDGSDKGetRegionInfoCompleted);
	FXDGCommonModule::OnXDGSDKGetXDGInfoJsonCompleted.AddUObject(this, &UServiceWidgetCommon::OnXDGSDKGetXDGInfoJsonCompleted);
#endif
}

void UXDServiceWidgetCommon::OnEnvironmentSelectChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	TUDebuger::IsTest = SelectedItem == UEnum::GetValueAsString(ETempDemoEnvironmentType::RND);
}

void UXDServiceWidgetCommon::OnXDGSDKInitCompleted(const bool success, const FString& result)
{
	DEMO_LOG(TEXT("OnXDGSDKInitCompleted: %s, %s"), success ? TEXT("True") : TEXT("False"), *result);
}

void UXDServiceWidgetCommon::OnXDGSDKGetXDGInfoJsonCompleted(const FString& result)
{
	DEMO_LOG(TEXT("OnXDGSDKGetXDGInfoJsonCompleted: %s"), *result);
}

void UXDServiceWidgetCommon::OnXDGSDKShareCompleted(const int32 code)
{
	DEMO_LOG(TEXT("OnXDGSDKShareCompleted: %d"), code);
}

void UXDServiceWidgetCommon::OnXDGSDKGetRegionInfoCompleted(const FString& countryCode, const FString& city,
                                                          const FString& timeZone, const FString& locationInfoType)
{
	DEMO_LOG(TEXT("OnXDGSDKGetRegionInfoCompleted: countryCode-%s city-%s timeZone-%s locationInfoType-%s"), *countryCode, *city, *timeZone, *locationInfoType);
}
