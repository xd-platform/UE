#include "XUAccountCancellationWidget.h"

#include "SWebBrowser.h"
#include "TUDebuger.h"
#include "TUSettings.h"
#include "XULanguageManager.h"
#include "XDUE.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UXUAccountCancellationWidget::Show(const FString& Url)
{
	if (UClass* MyWidgetClass = LoadClass<UXUAccountCancellationWidget>(
		nullptr, TEXT("WidgetBlueprint'/XDGSDK/BPAccountCancellation.BPAccountCancellation_C'")))
	{
		if (TUSettings::GetGameInstance().IsValid())
		{
			auto Widget = CreateWidget<
				UXUAccountCancellationWidget>(TUSettings::GetGameInstance().Get(), MyWidgetClass);
			Widget->Url = Url;
			Widget->AddToViewport(TUSettings::GetUILevel());
		}
	}
}

void UXUAccountCancellationWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TUDebuger::DisplayLog(Url);
	TB_Retry->SetText(FText::FromString(XULanguageManager::GetCurrentModel()->tds_retry));

	if (FPlatformMisc::GetNetworkConnectionType() == ENetworkConnectionType::None)
	{
		ShowErrorTipView(true);
		UpdateErrorTipView(true);
	}
	else
	{
		ShowErrorTipView(false);
	}
}

void UXUAccountCancellationWidget::NativeConstruct()
{
	Super::NativeConstruct();
	LoadURL(Url);
}

void UXUAccountCancellationWidget::GoBack()
{
	if (CanGoBack())
	{
		Super::GoBack();
	}
	else
	{
		Close();
	}
}

void UXUAccountCancellationWidget::Close()
{
	if (HasCancelAccount)
	{
		XDUE::OnLogout.Broadcast();
	}
	Super::Close();
}

void UXUAccountCancellationWidget::Reload()
{
	if (FPlatformMisc::GetNetworkConnectionType() == ENetworkConnectionType::None)
	{
		ShowErrorTipView(true);
		UpdateErrorTipView(true);
	}
	else
	{
		ShowErrorTipView(false);
		Super::Reload();
	}
}

void UXUAccountCancellationWidget::OnURLChanged(const FText& NewURL)
{
	Super::OnURLChanged(NewURL);
	TUDebuger::DisplayLog(NewURL.ToString());
	if (NewURL.ToString().Contains("success"))
	{
		HasCancelAccount = true;
		BTN_GoBack->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UXUAccountCancellationWidget::OnTitleChanged(const FText& NewTitle)
{
	Super::OnTitleChanged(NewTitle);
	TitleLabel->SetText(FText::FromString(NewTitle.ToString()));
}

void UXUAccountCancellationWidget::OnLoadError()
{
	Super::OnLoadError();
	ShowErrorTipView(true);
	UpdateErrorTipView(false);
}

void UXUAccountCancellationWidget::UpdateErrorTipView(bool IsNerworkError)
{
	auto langModel = XULanguageManager::GetCurrentModel();
	if (IsNerworkError)
	{
		ErrorTipIcon->SetBrushFromTexture(
			LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/XDGSDK/Images/LoadErrorRetry.LoadErrorRetry'")));
		TB_ErrorTip->SetText(FText::FromString(langModel->tds_net_error));
	}
	else
	{
		ErrorTipIcon->SetBrushFromTexture(
			LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/XDGSDK/Images/LoadFailRetry.LoadFailRetry'")));
		TB_ErrorTip->SetText(FText::FromString(langModel->tds_load_error));
	}
}

void UXUAccountCancellationWidget::ShowErrorTipView(bool IsShow)
{
	if (IsShow)
	{
		RetryPanel->SetVisibility(ESlateVisibility::Visible);
		GetInnerWebBrowser()->SetVisibility(EVisibility::Hidden);
	}
	else
	{
		RetryPanel->SetVisibility(ESlateVisibility::Hidden);
		GetInnerWebBrowser()->SetVisibility(EVisibility::Visible);
	}
}
