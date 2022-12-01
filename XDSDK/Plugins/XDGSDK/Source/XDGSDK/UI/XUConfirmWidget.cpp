// Fill out your copyright notice in the Description page of Project Settings.


#include "XUConfirmWidget.h"

#include "TUSettings.h"
#include "Components/Button.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

UXUConfirmWidget* UXUConfirmWidget::Create(const FText& InTitle, const FText& InContent, const FText& InBlueButtonText, const FText& InWhiteButtonText, bool bShowCloseButton, bool bAddToViewport)
{
	if (UClass* UIClass = LoadClass<UXUConfirmWidget>(nullptr, TEXT("WidgetBlueprint'/XDGSDK/BPConfirmWidget.BPConfirmWidget_C'")))
	{
		if (UGameInstance* GI = TUSettings::GetGameInstance().Get())
		{
			if (UXUConfirmWidget* UI = CreateWidget<UXUConfirmWidget>(GI, UIClass))
			{
				UI->ButtonSwitcher->SetActiveWidgetIndex(0);
				
				UI->Title->SetText(InTitle);
				UI->Content->SetText(InContent);
				UI->BlueButtonText->SetText(InBlueButtonText);
				UI->WhiteButtonText->SetText(InWhiteButtonText);
				UI->CloseButtonPanel->SetVisibility(bShowCloseButton ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
				
				if (bAddToViewport)
				{
					UI->AddToViewport(TUSettings::GetUILevel() + 5);
				}
				return UI;
			}
		}
	}
	return nullptr;
}

UXUConfirmWidget* UXUConfirmWidget::Create(const FText& InTitle, const FText& InContent, const FText& InBlueButtonText, bool bShowCloseButton, bool bAddToViewport)
{
	if (UClass* UIClass = LoadClass<UXUConfirmWidget>(nullptr, TEXT("WidgetBlueprint'/XDGSDK/BPConfirmWidget.BPConfirmWidget_C'")))
	{
		if (UGameInstance* GI = TUSettings::GetGameInstance().Get())
		{
			if (UXUConfirmWidget* UI = CreateWidget<UXUConfirmWidget>(GI, UIClass))
			{
				UI->ButtonSwitcher->SetActiveWidgetIndex(1);
				
				UI->Title->SetText(InTitle);
				UI->Content->SetText(InContent);
				UI->BlueButtonText_1->SetText(InBlueButtonText);
				UI->CloseButtonPanel->SetVisibility(bShowCloseButton ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

				if (bAddToViewport)
				{
					UI->AddToViewport(TUSettings::GetUILevel() + 5);
				}
				return UI;
			}
		}
	}
	return nullptr;
}

void UXUConfirmWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	WhiteButton->OnClicked.AddDynamic(this, &UXUConfirmWidget::OnWhiteButtonClickedEvent);
	BlueButton->OnClicked.AddDynamic(this, &UXUConfirmWidget::OnBlueButtonClickedEvent);
	BlueButton_1->OnClicked.AddDynamic(this, &UXUConfirmWidget::OnBlueButtonClickedEvent);
	CloseButton->OnClicked.AddDynamic(this, &UXUConfirmWidget::OnCloseButtonClickedEvent);
}

void UXUConfirmWidget::OnWhiteButtonClickedEvent()
{
	OnWhiteButtonClicked.ExecuteIfBound();
}

void UXUConfirmWidget::OnBlueButtonClickedEvent()
{
	OnBlueButtonClicked.ExecuteIfBound();
}

void UXUConfirmWidget::OnCloseButtonClickedEvent()
{
	OnCloseButtonClicked.ExecuteIfBound();
	RemoveFromParent();
}
