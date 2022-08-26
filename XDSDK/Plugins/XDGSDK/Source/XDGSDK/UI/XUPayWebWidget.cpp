#include "XUPayWebWidget.h"

#include "TUDebuger.h"
#include "TUSettings.h"
#include "URLParser.h"
#include "XULanguageManager.h"
#include "XDUE.h"

UXUPayWebWidget::UXUPayWebWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UXUPayWebWidget::Show(const FString& PayUrl, TFunction<void(XUType::PayResult Result)> CallBack) {
	if (UClass* MyWidgetClass = LoadClass<UXUPayWebWidget>(nullptr, TEXT("WidgetBlueprint'/XDGSDK/BPPayWeb.BPPayWeb_C'")))
	{
		if (TUSettings::GetGameInstance().IsValid()) {
			auto Widget = CreateWidget<UXUPayWebWidget>(TUSettings::GetGameInstance().Get(), MyWidgetClass);
			if (PayUrl.Contains("?")) { // 加个时间戳，用于浏览器清除缓存
				Widget->PayUrl = PayUrl + FString::Printf(TEXT("&timestamp=%lld"), FDateTime::UtcNow().ToUnixTimestamp());
			} else {
				Widget->PayUrl = PayUrl + FString::Printf(TEXT("?timestamp=%lld"), FDateTime::UtcNow().ToUnixTimestamp());
			}
			Widget->CallBack = CallBack;
			Widget->AddToViewport(TUSettings::GetUILevel());
		}
	}
}



void UXUPayWebWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PayWebBrowser->LoadURL(PayUrl);
	CloseButton->OnClicked.AddUniqueDynamic(this, &UXUPayWebWidget::OnCloseClick);
	RetryBtn->OnClicked.AddUniqueDynamic(this, &UXUPayWebWidget::OnRetryBtnClick);
	PayWebBrowser->OnUrlChanged.AddUniqueDynamic(this, &UXUPayWebWidget::OnUrlChanged);
	PayWebBrowser->OnLoadError.AddUniqueDynamic(this, &UXUPayWebWidget::OnWebLoadError);
	RetryBtnLabel->SetText(FText::FromString(XULanguageManager::GetCurrentModel()->tds_retry));

	if (FPlatformMisc::GetNetworkConnectionType() == ENetworkConnectionType::None) {
		ShowErrorTipView(true);
		UpdateErrorTipView(true);
	} else {
		ShowErrorTipView(false);
	}
}

void UXUPayWebWidget::OnCloseClick() {
	CloseWithResult(XUType::PayCancel);
}

void UXUPayWebWidget::OnRetryBtnClick() {
	if (FPlatformMisc::GetNetworkConnectionType() == ENetworkConnectionType::None) {
		ShowErrorTipView(true);
		UpdateErrorTipView(true);
	} else {
		ShowErrorTipView(false);
		PayWebBrowser->LoadURL(PayUrl);
	}
}

void UXUPayWebWidget::OnUrlChanged(const FString& Text) {
	TUDebuger::DisplayLog(Text);
	auto Parse = TUCommon::FURL_RFC3986();
	Parse.Parse(Text);
	FString Result = Parse.GetFragment();
	if (Result == "success") {
		CloseWithResult(XUType::PaySuccess);
	} else if (Result == "fail") {
		CloseWithResult(XUType::PayFail);
	} else if (Result == "cancel") {
		CloseWithResult(XUType::PayCancel);
	}
}

void UXUPayWebWidget::OnWebLoadError() {
	ShowErrorTipView(true);
	UpdateErrorTipView(false);
}

void UXUPayWebWidget::CloseWithResult(XUType::PayResult Result) {
	if (CallBack) {
		CallBack(Result);
	}
	RemoveFromParent();
}
void UXUPayWebWidget::UpdateErrorTipView(bool IsNerworkError) {
	auto langModel = XULanguageManager::GetCurrentModel();
	if (IsNerworkError) {
		ErrorTipImage->SetBrushFromTexture(LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/XDGSDK/Images/LoadErrorRetry.LoadErrorRetry'")));
		ErrorTipLabel->SetText(FText::FromString(langModel->tds_net_error));
	} else {
		ErrorTipImage->SetBrushFromTexture(LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/XDGSDK/Images/LoadFailRetry.LoadFailRetry'")));
		ErrorTipLabel->SetText(FText::FromString(langModel->tds_load_error));
	}
}

void UXUPayWebWidget::ShowErrorTipView(bool IsShow) {
	if (IsShow) {
		ErrorBox->SetVisibility(ESlateVisibility::Visible);
		PayWebBrowser->SetVisibility(ESlateVisibility::Hidden);
	} else {
		ErrorBox->SetVisibility(ESlateVisibility::Hidden);
		PayWebBrowser->SetVisibility(ESlateVisibility::Visible);
	}
}



