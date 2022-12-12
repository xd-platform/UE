// Fill out your copyright notice in the Description page of Project Settings.


#include "XUPrivacyWidget.h"
#include "SWebBrowser.h"
#include "TapCommon.h"
#include "XULanguageManager.h"
#include "TUHelper.h"
#include "TUHUD.h"
#include "TUSettings.h"
#include "XUConfigManager.h"
#include "XUPrivacyDisagreeWidget.h"
#include "Components/CheckBox.h"
#include "Agreement/XUAgreementManager.h"


void UXUPrivacyWidget::ShowPrivacy(TFunction<void()> Completed)
{
	if (UClass* MyWidgetClass = LoadClass<UXUPrivacyWidget>(
		nullptr, TEXT("WidgetBlueprint'/XDGSDK/BPPrivacyUI.BPPrivacyUI_C'")))
	{
		if (TUSettings::GetGameInstance().IsValid())
		{
			UXUPrivacyWidget* widget = CreateWidget<UXUPrivacyWidget>(TUSettings::GetGameInstance().Get(),
			                                                          MyWidgetClass);
			widget->Completed = Completed;
			widget->AddToViewport(TUSettings::GetUILevel());
		}
	}
}

void UXUPrivacyWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AmericaCheckBox->OnCheckStateChanged.AddDynamic(this, &UXUPrivacyWidget::OnCheckStateChanged);
	KrAgeCheckBox->OnCheckStateChanged.AddDynamic(this, &UXUPrivacyWidget::OnCheckStateChanged);
	KrPushCheckBox->OnCheckStateChanged.AddDynamic(this, &UXUPrivacyWidget::OnCheckStateChanged);

	ComfirmButton->OnClicked.AddDynamic(this, &UXUPrivacyWidget::OnConfirmBtnClick);
	DeclineButton->OnClicked.AddDynamic(this, &UXUPrivacyWidget::OnDeclineBtnClick);
	KrPushCheckProtocolButton->OnClicked.AddDynamic(this, &UXUPrivacyWidget::OnKrPushCheckProtocolButtonClick);
	
	OriginURL = XUAgreementManager::GetAgreementUrl();
	// OriginURL = "https://protocol.xd.com/sdk/merger-test.html?language=ru_RU";
	if (OriginURL.Contains("?"))
	{
		// 加个时间戳，用于浏览器清除缓存
		OriginURL += FString::Printf(TEXT("&timestamp=%lld"), FDateTime::UtcNow().ToUnixTimestamp());
	}
	else
	{
		OriginURL += FString::Printf(TEXT("?timestamp=%lld"), FDateTime::UtcNow().ToUnixTimestamp());
	}
	LoadURL(OriginURL);
	LoadURL(OriginURL); // 国内协议加载过快，会有布局问题，所以这里连续调用两遍临时解决下

	TSharedPtr<FXULanguageModel> langModel = XULanguageManager::GetCurrentModel();
	ComfirmButtonLabel->SetText(FText::FromString(langModel->xd_agreement_agree));
	DeclineButtonLabel->SetText(FText::FromString(langModel->xd_agreement_disagree));
	TB_ErrorTip->SetText(FText::FromString(langModel->xd_agreement_load_failed));

	if (IsInKrAndPushEnable())
	{
		KrPushCheckLabel->SetText(FText::FromString(langModel->tds_push_agreement));
		TArray<FStringFormatArg> AgeArg = {FStringFormatArg(14)};
		auto AgeText = FString::Format(*langModel->tds_is_adult_agreement, AgeArg);
		KrAgeCheckLabel->SetText(FText::FromString(AgeText));

		FString ServiceAgree = langModel->tds_service_agree;
		FString ServiceAgreeProtocol = langModel->tds_service_user_info;
		auto Index = ServiceAgree.Find(*ServiceAgreeProtocol);
		if (Index != INDEX_NONE) {
			ServiceAgree.InsertAt(Index + ServiceAgreeProtocol.Len(), "</>");
			ServiceAgree.InsertAt(Index, "<KRProtocol>");
		}
		KrPushCheckLabel->SetText(FText::FromString(ServiceAgree));
		// KrPushCheckProtocolLabel->SetText(FText::FromString(langModel->tds_protocol_kr));
		KrAgeCheckTagLabel->SetText(FText::FromString(FString::Printf(TEXT("%s"), *langModel->tds_service_required)));
		KrPushCheckTagLabel->SetText(FText::FromString(FString::Printf(TEXT("%s"), *langModel->tds_service_optional)));
	}
	else if (IsInNorthAmerica())
	{
		TArray<FStringFormatArg> AgeArg = {FStringFormatArg(16)};
		auto AgeText = FString::Format(*langModel->tds_is_adult_agreement, AgeArg);
		AmericaCheckLabel->SetText(FText::FromString(AgeText));
	}

	UpdateComfirmBtnState();
}

void UXUPrivacyWidget::OnCheckStateChanged(bool isChecked)
{
	UpdateComfirmBtnState();
}

void UXUPrivacyWidget::OnConfirmBtnClick()
{
	if (IsInNorthAmerica() && !AmericaCheckBox->IsChecked())
	{
		FTapCommonModule::TapThrobberShowToast(XULanguageManager::GetCurrentModel()->xd_agreement_age_tips);
		return;
	}

	if (IsInKrAndPushEnable() && !KrAgeCheckBox->IsChecked())
	{
		FTapCommonModule::TapThrobberShowToast(XULanguageManager::GetCurrentModel()->xd_agreement_age_tips);
		return;
	}

	if (IsInKrAndPushEnable())
	{
		XUConfigManager::RecordKRPushSetting(KrPushCheckBox->IsChecked());
	}
	if (Completed)
	{
		Completed();
	}
	RemoveFromParent();
}

void UXUPrivacyWidget::OnDeclineBtnClick()
{
	UXUPrivacyDisagreeWidget::Show();
}

void UXUPrivacyWidget::OnKrPushCheckProtocolButtonClick() {
	auto Ptr = XUAgreementManager::GetCurrentAgreement();
	if (Ptr.IsValid()) {
		TUHelper::LaunchURL(*Ptr->dataCollectionAgreementUrl, NULL, NULL);
	} else {
		TUDebuger::WarningLog("Current Agreement Is Null");
	}
}

void UXUPrivacyWidget::Reload()
{
	Super::Reload();
}

void UXUPrivacyWidget::OnLoadStarted()
{
	Super::OnLoadStarted();

	GetInnerWebBrowser()->SetVisibility(EVisibility::Visible);
	ComfirmButton->SetVisibility(ESlateVisibility::Hidden);
	BTN_Retry->SetVisibility(ESlateVisibility::Collapsed);
	DeclineButton->SetVisibility(ESlateVisibility::Hidden);
	KrPushCheckBox->SetVisibility(ESlateVisibility::Collapsed);
	KrAgeCheckBox->SetVisibility(ESlateVisibility::Collapsed);
	AmericaCheckBox->SetVisibility(ESlateVisibility::Collapsed);

}

void UXUPrivacyWidget::OnLoadCompleted()
{
	Super::OnLoadCompleted();

	TUDebuger::DisplayLog(NavigationUrl);
	TUDebuger::DisplayLog("Privacy Web Load Completed");
	
	GetInnerWebBrowser()->SetVisibility(EVisibility::Visible);
	ComfirmButton->SetVisibility(ESlateVisibility::Visible);
	BTN_Retry->SetVisibility(ESlateVisibility::Collapsed);
	DeclineButton->SetVisibility(ESlateVisibility::Visible);

	AmericaCheckBox->SetVisibility(ESlateVisibility::Collapsed);
	KrAgeCheckBox->SetVisibility(ESlateVisibility::Collapsed);
	KrPushCheckBox->SetVisibility(ESlateVisibility::Collapsed);
	if (IsInNorthAmerica()) {
		AmericaCheckBox->SetVisibility(ESlateVisibility::Visible);
	}
	else if (IsInKrAndPushEnable()) {
		KrAgeCheckBox->SetVisibility(ESlateVisibility::Visible);
		KrPushCheckBox->SetVisibility(ESlateVisibility::Visible);
	}
}

void UXUPrivacyWidget::OnLoadError()
{
	Super::OnLoadError();

	TUDebuger::DisplayLog(NavigationUrl);
	TUDebuger::DisplayLog("Privacy Web Load Error");
	if (NavigationUrl == OriginURL)
	{
		GetInnerWebBrowser()->SetVisibility(EVisibility::Collapsed);
		ComfirmButton->SetVisibility(ESlateVisibility::Hidden);
		BTN_Retry->SetVisibility(ESlateVisibility::Visible);
		DeclineButton->SetVisibility(ESlateVisibility::Hidden);
		KrPushCheckBox->SetVisibility(ESlateVisibility::Collapsed);
		KrAgeCheckBox->SetVisibility(ESlateVisibility::Collapsed);
		AmericaCheckBox->SetVisibility(ESlateVisibility::Collapsed);
	}
}

bool UXUPrivacyWidget::OnBeforeNavigation(const FString& URL, const FWebNavigationRequest& Request)
{
	TUDebuger::DisplayLog("OnWebBeforeNavigation");
	NavigationUrl = URL;
	if (OriginURL == URL)
	{
		return false;
	}
	else
	{
		TUHelper::LaunchURL(*URL, nullptr, nullptr);
		ComfirmButton->SetVisibility(ESlateVisibility::Visible);
		DeclineButton->SetVisibility(ESlateVisibility::Visible);
		return true;
	}
}

void UXUPrivacyWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) {
}

bool UXUPrivacyWidget::IsInKrAndPushEnable()
{
	return XUConfigManager::IsGameInKoreaAndPushServiceEnable();
}

bool UXUPrivacyWidget::IsInNorthAmerica()
{
	return XUConfigManager::IsGameInNA();
}

void UXUPrivacyWidget::UpdateComfirmBtnState()
{
	if ((IsInNorthAmerica() && !AmericaCheckBox->IsChecked()) || (IsInKrAndPushEnable() && !KrAgeCheckBox->IsChecked()))
	{
		ComfirmButton->WidgetStyle.SetNormal(ComfirmDisableBrush);
		ComfirmButton->WidgetStyle.SetHovered(ComfirmDisableBrush);
		ComfirmButton->WidgetStyle.SetPressed(ComfirmDisableBrush);
	}
	else
	{
		ComfirmButton->WidgetStyle.SetNormal(ComfirmBrush);
		ComfirmButton->WidgetStyle.SetHovered(ComfirmHoverBrush);
		ComfirmButton->WidgetStyle.SetPressed(ComfirmHoverBrush);
	}
}
