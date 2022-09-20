#include "XUPayWebWidget.h"

#include "SWebBrowser.h"
#include "TUDebuger.h"
#include "TUSettings.h"
#include "URLParser.h"
#include "XULanguageManager.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"

void UXUPayWebWidget::Show(const FString& PayUrl, TFunction<void(XUType::PayResult Result)> CallBack)
{
	if (UClass* MyWidgetClass = LoadClass<UXUPayWebWidget>(
		nullptr, TEXT("WidgetBlueprint'/XDGSDK/BPPayWeb.BPPayWeb_C'")))
	{
		if (TUSettings::GetGameInstance().IsValid())
		{
			auto Widget = CreateWidget<UXUPayWebWidget>(TUSettings::GetGameInstance().Get(), MyWidgetClass);
			if (PayUrl.Contains("?"))
			{
				// 加个时间戳，用于浏览器清除缓存
				Widget->PayUrl = PayUrl + FString::Printf(
					TEXT("&timestamp=%lld"), FDateTime::UtcNow().ToUnixTimestamp());
			}
			else
			{
				Widget->PayUrl = PayUrl + FString::Printf(
					TEXT("?timestamp=%lld"), FDateTime::UtcNow().ToUnixTimestamp());
			}
			Widget->CallBack = CallBack;
			Widget->AddToViewport(TUSettings::GetUILevel());
		}
	}
}

void UXUPayWebWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

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

void UXUPayWebWidget::NativeConstruct()
{
	Super::NativeConstruct();
	LoadURL(PayUrl);
}

void UXUPayWebWidget::Close()
{
	Super::Close();
	CloseWithResult(XUType::PayCancel);
}

void UXUPayWebWidget::Reload()
{
	if (FPlatformMisc::GetNetworkConnectionType() == ENetworkConnectionType::None)
	{
		ShowErrorTipView(true);
		UpdateErrorTipView(true);
	}
	else
	{
		ShowErrorTipView(false);
	}
	Super::Reload();
}

void UXUPayWebWidget::OnURLChanged(const FText& NewURL)
{
	Super::OnURLChanged(NewURL);
	TUDebuger::DisplayLog(NewURL.ToString());
	auto Parse = TUCommon::FURL_RFC3986();
	Parse.Parse(NewURL.ToString());
	FString Result = Parse.GetFragment();
	if (Result == "success")
	{
		CloseWithResult(XUType::PaySuccess);
	}
	else if (Result == "fail")
	{
		CloseWithResult(XUType::PayFail);
	}
	else if (Result == "cancel")
	{
		CloseWithResult(XUType::PayCancel);
	}
}

void UXUPayWebWidget::OnLoadStarted()
{
	Super::OnLoadStarted();
}

void UXUPayWebWidget::OnLoadCompleted()
{
	Super::OnLoadCompleted();
}

void UXUPayWebWidget::OnLoadError()
{
	Super::OnLoadError();
	ShowErrorTipView(true);
	UpdateErrorTipView(false);
}

void UXUPayWebWidget::CloseWithResult(XUType::PayResult Result)
{
	if (CallBack)
	{
		CallBack(Result);
	}
	RemoveFromParent();
}

void UXUPayWebWidget::UpdateErrorTipView(bool IsNerworkError)
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

void UXUPayWebWidget::ShowErrorTipView(bool IsShow)
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
