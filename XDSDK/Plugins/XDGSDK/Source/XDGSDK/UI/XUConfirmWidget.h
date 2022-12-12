// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "XUConfirmWidget.generated.h"

class UWidgetSwitcher;
class URichTextBlock;
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS(Abstract)
class XDGSDK_API UXUConfirmWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	static UXUConfirmWidget* Create(const FText& InTitle, const FText& InContent, const FText& InBlueButtonText, const FText& InWhiteButtonText, bool bShowCloseButton = false, bool bAddToViewport = false);
	static UXUConfirmWidget* Create(const FText& InTitle, const FText& InContent, const FText& InBlueButtonText, bool bShowCloseButton = false, bool bAddToViewport = false);
	
	FSimpleDelegate OnWhiteButtonClicked;
	FSimpleDelegate OnBlueButtonClicked;
	FSimpleDelegate OnCloseButtonClicked;
	
protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnWhiteButtonClickedEvent();

	UFUNCTION()
	void OnBlueButtonClickedEvent();

	UFUNCTION()
	void OnCloseButtonClickedEvent();

	UPROPERTY(Meta = (BindWidget))
	UButton* WhiteButton;

	UPROPERTY(Meta = (BindWidget))
	UButton* BlueButton;

	UPROPERTY(Meta = (BindWidget))
	UButton* CloseButton;

	UPROPERTY(Meta = (BindWidget))
	UWidgetSwitcher* ButtonSwitcher;

	UPROPERTY(Meta = (BindWidget))
	UButton* BlueButton_1;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* BlueButtonText_1;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Title;

	UPROPERTY(Meta = (BindWidget))
	URichTextBlock* Content;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* WhiteButtonText;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* BlueButtonText;
};
