// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Object.h"
#include "XUQuitAccountWidget.generated.h"


class UButton;
/**
 * 
 */
UCLASS()
class XDGSDK_API UXUQuitAccountWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	FSimpleDelegate OnConfirm;
	FSimpleDelegate OnCancel;
	
protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnConfirmClicked();

	UFUNCTION()
	void OnCancelClicked();

	UPROPERTY(Meta = (BindWidget))
	UButton* ConfirmButton;

	UPROPERTY(Meta = (BindWidget))
	UButton* CancelButton;

	UPROPERTY(Meta = (BindWidget))
	UButton* CloseButton;
};
