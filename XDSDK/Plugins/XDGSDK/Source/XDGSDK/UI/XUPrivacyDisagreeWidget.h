// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "XUPrivacyDisagreeWidget.generated.h"

/**
 *  
 */
UCLASS()
class XDGSDK_API UXUPrivacyDisagreeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	 UXUPrivacyDisagreeWidget(const FObjectInitializer& ObjectInitializer);

	static void Show();

protected:

	virtual void NativeConstruct() override;
	

	UFUNCTION()
	void OnComfirmBtnClick();

	UFUNCTION()
	void OnDeclineBtnClick();

	
private:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ContentLabel;

	UPROPERTY(meta = (BindWidget))
	UButton* ComfirmButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ComfirmButtonLabel;

	UPROPERTY(meta = (BindWidget))
	UButton* DeclineButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DeclineButtonLabel;
	
};
