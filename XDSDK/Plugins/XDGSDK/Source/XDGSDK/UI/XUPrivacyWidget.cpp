// Fill out your copyright notice in the Description page of Project Settings.


#include "XUPrivacyWidget.h"

#include "SWebBrowser.h"
#include "TapCommonBPLibrary.h"
#include "XULanguageManager.h"
#include "TUHelper.h"
#include "TUHUD.h"
#include "TUSettings.h"
#include "XUConfigManager.h"
#include "XUPrivacyDisagreeWidget.h"
#include "Components/CheckBox.h"


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

	AdditionalCheckBox->OnCheckStateChanged.AddDynamic(this, &UXUPrivacyWidget::OnCheckStateChanged);

	ComfirmButton->OnClicked.AddDynamic(this, &UXUPrivacyWidget::OnConfirmBtnClick);
	DeclineButton->OnClicked.AddDynamic(this, &UXUPrivacyWidget::OnDeclineBtnClick);

	OriginURL = XUConfigManager::GetAgreementUrl();
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
		AdditionalCheckLabel->SetText(FText::FromString(langModel->tds_push_agreement));
	}
	else if (IsInNorthAmerica())
	{
		AdditionalCheckLabel->SetText(FText::FromString(langModel->tds_is_adult_agreement));
	}

	UpdateComfirmBtnState();
}

void UXUPrivacyWidget::OnCheckStateChanged(bool isChecked)
{
	UpdateComfirmBtnState();
}

void UXUPrivacyWidget::OnConfirmBtnClick()
{
	if (IsInNorthAmerica() && !AdditionalCheckBox->IsChecked())
	{
		UTUHUD::ShowToast(XULanguageManager::GetCurrentModel()->xd_agreement_age_tips);
		return;
	}

	if (IsInKrAndPushEnable())
	{
		XUConfigManager::RecordKRPushSetting(AdditionalCheckBox->IsChecked());
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
	AdditionalCheckBox->SetVisibility(ESlateVisibility::Collapsed);
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
	if (IsInKrAndPushEnable() || IsInNorthAmerica())
	{
		AdditionalCheckBox->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		AdditionalCheckBox->SetVisibility(ESlateVisibility::Collapsed);
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
		AdditionalCheckBox->SetVisibility(ESlateVisibility::Collapsed);
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
		UTapCommonBPLibrary::LaunchURL(*URL, nullptr, nullptr);
		ComfirmButton->SetVisibility(ESlateVisibility::Visible);
		DeclineButton->SetVisibility(ESlateVisibility::Visible);
		return true;
	}
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
	if (IsInNorthAmerica() && !AdditionalCheckBox->IsChecked())
	{
		ComfirmButtonImage->SetBrushFromTexture(
			LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/TapLogin/Image/taptap-router-gray.taptap-router-gray'")));
	}
	else
	{
		ComfirmButtonImage->SetBrushFromTexture(
			LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/TapLogin/Image/taptap-router.taptap-router'")));
	}
}
