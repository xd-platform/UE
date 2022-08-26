#include "XUAccountCancellationWidget.h"

#include "TUDebuger.h"
#include "TUSettings.h"
#include "URLParser.h"
#include "XULanguageManager.h"
#include "XDUE.h"

UXUAccountCancellationWidget::UXUAccountCancellationWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UXUAccountCancellationWidget::Show(const FString& Url) {
	if (UClass* MyWidgetClass = LoadClass<UXUAccountCancellationWidget>(nullptr, TEXT("WidgetBlueprint'/XDGSDK/BPAccountCancellation.BPAccountCancellation_C'")))
	{
		if (TUSettings::GetGameInstance().IsValid()) {
			auto Widget = CreateWidget<UXUAccountCancellationWidget>(TUSettings::GetGameInstance().Get(), MyWidgetClass);
			Widget->Url = Url;
			Widget->AddToViewport(TUSettings::GetUILevel());
		}
	}
}


void UXUAccountCancellationWidget::NativeConstruct()
{
	Super::NativeConstruct();

	WebBrowser->LoadURL(Url);
	TUDebuger::DisplayLog(Url);
	CloseBtn->OnClicked.AddUniqueDynamic(this, &UXUAccountCancellationWidget::OnCloseClick);
	BackBtn->OnClicked.AddUniqueDynamic(this, &UXUAccountCancellationWidget::OnBackClick);
	RetryBtn->OnClicked.AddUniqueDynamic(this, &UXUAccountCancellationWidget::OnRetryBtnClick);
	WebBrowser->OnUrlChanged.AddUniqueDynamic(this, &UXUAccountCancellationWidget::OnUrlChanged);
	WebBrowser->OnLoadError.AddUniqueDynamic(this, &UXUAccountCancellationWidget::OnWebLoadError);
	WebBrowser->OnTitleChanged.AddUniqueDynamic(this, &UXUAccountCancellationWidget::OnTitleChanged);
	RetryBtnLabel->SetText(FText::FromString(XULanguageManager::GetCurrentModel()->tds_retry));

	if (FPlatformMisc::GetNetworkConnectionType() == ENetworkConnectionType::None) {
		ShowErrorTipView(true);
		UpdateErrorTipView(true);
	} else {
		ShowErrorTipView(false);
	}
}

void UXUAccountCancellationWidget::OnCloseClick() {
	if (HasCancelAccount) {
		XDUE::OnLogout.Broadcast();
	}
	RemoveFromParent();
}

void UXUAccountCancellationWidget::OnBackClick() {
	if (WebBrowser->CanGoBack()) {
		WebBrowser->GoBack();
	} else {
		OnCloseClick();
	}
}

void UXUAccountCancellationWidget::OnRetryBtnClick() {
	if (FPlatformMisc::GetNetworkConnectionType() == ENetworkConnectionType::None) {
		ShowErrorTipView(true);
		UpdateErrorTipView(true);
	} else {
		ShowErrorTipView(false);
		WebBrowser->LoadURL(Url);
	}
}

void UXUAccountCancellationWidget::OnUrlChanged(const FString& Text) {
	TUDebuger::DisplayLog(Text);
	if (Text.Contains("success")) {
		HasCancelAccount = true;
		BackBtn->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UXUAccountCancellationWidget::OnTitleChanged(const FString& Text) {
	TitleLabel->SetText(FText::FromString(Text));
}

void UXUAccountCancellationWidget::OnWebLoadError() {
	ShowErrorTipView(true);
	UpdateErrorTipView(false);
}

void UXUAccountCancellationWidget::UpdateErrorTipView(bool IsNerworkError) {
	auto langModel = XULanguageManager::GetCurrentModel();
	if (IsNerworkError) {
		ErrorTipImage->SetBrushFromTexture(LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/XDGSDK/Images/LoadErrorRetry.LoadErrorRetry'")));
		ErrorTipLabel->SetText(FText::FromString(langModel->tds_net_error));
	} else {
		ErrorTipImage->SetBrushFromTexture(LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/XDGSDK/Images/LoadFailRetry.LoadFailRetry'")));
		ErrorTipLabel->SetText(FText::FromString(langModel->tds_load_error));
	}
}

void UXUAccountCancellationWidget::ShowErrorTipView(bool IsShow) {
	if (IsShow) {
		ErrorBox->SetVisibility(ESlateVisibility::Visible);
		WebBrowser->SetVisibility(ESlateVisibility::Hidden);
	} else {
		ErrorBox->SetVisibility(ESlateVisibility::Hidden);
		WebBrowser->SetVisibility(ESlateVisibility::Visible);
	}
}



