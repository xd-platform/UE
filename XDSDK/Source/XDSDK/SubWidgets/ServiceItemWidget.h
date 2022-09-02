// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "ServiceItemWidget.generated.h"

class FOnButtonClickedEvent;
class UTextBlock;

/**
 * 
 */
UCLASS(Abstract)
class XDSDK_API UServiceItemWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	FORCEINLINE UButton* GetClickButton() const { return Button; }
	
protected:
	virtual void NativePreConstruct() override;
	
	UPROPERTY(Meta = (BindWidget))
	UButton* Button;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* ButtonText;

	UPROPERTY(EditAnywhere)
	FText ButtonDisplayText;
};
