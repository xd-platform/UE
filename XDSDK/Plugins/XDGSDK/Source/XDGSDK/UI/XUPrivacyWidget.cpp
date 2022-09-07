// Fill out your copyright notice in the Description page of Project Settings.


#include "XUPrivacyWidget.h"

#include "TapCommonBPLibrary.h"
#include "XULanguageManager.h"
#include "TUHelper.h"
#include "TUHUD.h"
#include "TUSettings.h"
#include "XUConfigManager.h"
#include "XUPrivacyDisagreeWidget.h"


UXUPrivacyWidget::UXUPrivacyWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UXUPrivacyWidget::ShowPrivacy(TFunction<void()> Completed)
{
	if (UClass* MyWidgetClass = LoadClass<UXUPrivacyWidget>(nullptr, TEXT("WidgetBlueprint'/XDGSDK/BPPrivacyUI.BPPrivacyUI_C'")))
	{
		if (TUSettings::GetGameInstance().IsValid()) {
			auto widget = CreateWidget<UXUPrivacyWidget>(TUSettings::GetGameInstance().Get(), MyWidgetClass);
			widget->Completed = Completed;
			widget->AddToViewport(TUSettings::GetUILevel());
		}
	}
}

void UXUPrivacyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	AdditionalCheckBox->OnCheckStateChanged.AddUniqueDynamic(this, &UXUPrivacyWidget::OnCheckStateChanged);

	FScriptDelegate BtnDel;
	BtnDel.BindUFunction(this, "OnComfirmBtnClick");
	ComfirmButton->OnClicked.Add(BtnDel);
	DeclineButton->OnClicked.AddUniqueDynamic(this, &UXUPrivacyWidget::OnDeclineBtnClick);
	LoadErrorBtn->OnClicked.AddUniqueDynamic(this, &UXUPrivacyWidget::OnLoadErrorBtnClick);

	// OriginURL =  XUConfigManager::GetAgreementUrl();
	OriginURL = "https://protocol.xd.com/sdk/merger-test.html?language=ru_RU";
	if (OriginURL.Contains("?")) { // 加个时间戳，用于浏览器清除缓存
		OriginURL += FString::Printf(TEXT("&timestamp=%lld"), FDateTime::UtcNow().ToUnixTimestamp());
	} else {
		OriginURL += FString::Printf(TEXT("?timestamp=%lld"), FDateTime::UtcNow().ToUnixTimestamp());
	}
	PrivacyWebBrowser->LoadURL(OriginURL);
	PrivacyWebBrowser->LoadURL(OriginURL);// 国内协议加载过快，会有布局问题，所以这里连续调用两遍临时解决下
	
	auto langModel = XULanguageManager::GetCurrentModel();
	ComfirmButtonLabel->SetText(FText::FromString(langModel->xd_agreement_agree));
	DeclineButtonLabel->SetText(FText::FromString(langModel->xd_agreement_disagree));
	LoadErrorLabel->SetText(FText::FromString(langModel->xd_agreement_load_failed));

	if (IsInKrAndPushEnable()) {
		AdditionalCheckLabel->SetText(FText::FromString(langModel->tds_push_agreement));
	} else if (IsInNorthAmerica()) {
		AdditionalCheckLabel->SetText(FText::FromString(langModel->tds_is_adult_agreement));
	} 

	PrivacyWebBrowser->OnLoadCompleted.AddUniqueDynamic(this, &UXUPrivacyWidget::OnWebLoadCompleted);
	PrivacyWebBrowser->OnLoadError.AddUniqueDynamic(this, &UXUPrivacyWidget::OnWebLoadError);
	PrivacyWebBrowser->OnBeforeNavigation.BindUObject(this, &UXUPrivacyWidget::OnWebBeforeNavigation);

	UpdateUI(Loading);
	UpdateComfirmBtnState();
}

void UXUPrivacyWidget::OnCheckStateChanged(bool isChecked)
{
	UpdateComfirmBtnState();
}

void UXUPrivacyWidget::OnComfirmBtnClick()
{
	if (IsInNorthAmerica() && !AdditionalCheckBox->IsChecked()) {
		UTUHUD::ShowToast(XULanguageManager::GetCurrentModel()->xd_agreement_age_tips);
		return;
	} 

	if (IsInKrAndPushEnable()) {
		XUConfigManager::RecordKRPushSetting(AdditionalCheckBox->IsChecked());
	}
	if (Completed) {
		Completed();
	}
	RemoveFromParent();
}

void UXUPrivacyWidget::OnLoadErrorBtnClick() {
	PrivacyWebBrowser->LoadURL(OriginURL);
	UpdateUI(Loading);
}

void UXUPrivacyWidget::OnDeclineBtnClick() {
	UXUPrivacyDisagreeWidget::Show();
}

void UXUPrivacyWidget::OnWebLoadCompleted() {
	TUDebuger::DisplayLog(NavigationUrl);
	TUDebuger::DisplayLog("Privacy Web Load Completed");
	if (NavigationUrl == OriginURL) {
		UpdateUI(LoadSuccess);
	}
}

void UXUPrivacyWidget::OnWebLoadError() {
	TUDebuger::DisplayLog(NavigationUrl);
	TUDebuger::DisplayLog("Privacy Web Load Error");
	if (NavigationUrl == OriginURL) {
		UpdateUI(LoadError);
	}
}

bool UXUPrivacyWidget::OnWebBeforeNavigation(const FString& Url, const FWebNavigationRequest& Request) {
	TUDebuger::DisplayLog("OnWebBeforeNavigation");
	NavigationUrl = Url;
	if (OriginURL == Url) {
		return false;
	}  else {
		UTapCommonBPLibrary::LaunchURL(*Url, nullptr, nullptr);
		return true;
	}
}

bool UXUPrivacyWidget::IsInKrAndPushEnable() {
	return XUConfigManager::IsGameInKoreaAndPushServiceEnable();
}

bool UXUPrivacyWidget::IsInNorthAmerica() {
	return XUConfigManager::IsGameInNA();
}

void UXUPrivacyWidget::UpdateComfirmBtnState() {
	if (IsInNorthAmerica() && !AdditionalCheckBox->IsChecked()) {
		ComfirmButtonImage->SetBrushFromTexture(LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/TapLogin/Image/taptap-router-gray.taptap-router-gray'")));
	} else {
		ComfirmButtonImage->SetBrushFromTexture(LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/TapLogin/Image/taptap-router.taptap-router'")));
	}
}

void UXUPrivacyWidget::UpdateUI(LoadState State) {
	switch (State) {
	case Loading:
		PrivacyWebBrowser->SetVisibility(ESlateVisibility::Visible);
		ComfirmButton->SetVisibility(ESlateVisibility::Hidden);
		LoadErrorBtn->SetVisibility(ESlateVisibility::Collapsed);
		DeclineButton->SetVisibility(ESlateVisibility::Hidden);
		AdditionalCheckBox->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case LoadError:
		PrivacyWebBrowser->SetVisibility(ESlateVisibility::Collapsed);
		ComfirmButton->SetVisibility(ESlateVisibility::Hidden);
		LoadErrorBtn->SetVisibility(ESlateVisibility::Visible);
		DeclineButton->SetVisibility(ESlateVisibility::Hidden);
		AdditionalCheckBox->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case LoadSuccess:
		PrivacyWebBrowser->SetVisibility(ESlateVisibility::Visible);
		ComfirmButton->SetVisibility(ESlateVisibility::Visible);
		LoadErrorBtn->SetVisibility(ESlateVisibility::Collapsed);
		DeclineButton->SetVisibility(ESlateVisibility::Visible);
		if (IsInKrAndPushEnable() || IsInNorthAmerica()) {
			AdditionalCheckBox->SetVisibility(ESlateVisibility::Visible);
		} else {
			AdditionalCheckBox->SetVisibility(ESlateVisibility::Collapsed);
		}
		break;
	default: ;
	}
}


