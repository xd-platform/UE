// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "XDButton.generated.h"

/**
 * 
 */
UCLASS()
class XDGSDK_API UXDButton : public UButton
{
	GENERATED_BODY()
	/** The button style used at runtime */
public:

#if WITH_EDITOR
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="XDSettings", meta=( DisplayName="HighLight" ))
	float HighLight = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="XDSettings", meta=( DisplayName="DefaultBrush" ))
	FSlateBrush DefaultBrush;
#endif

protected:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
	virtual const FText GetPaletteCategory() override;
#endif
	
};
