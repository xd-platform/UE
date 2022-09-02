// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EditableTextItemWidget.generated.h"

class UEditableTextBox;
class UButton;


DECLARE_DELEGATE_OneParam(FOnItemRequestRemoved, class UEditableTextItemWidget*);

/**
 * 
 */
UCLASS(Abstract)
class XDSDK_API UEditableTextItemWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void AppendJsonObject(const TSharedPtr<FJsonObject>& InOutJsonObject);

	void AppendStringArray(TArray<FString>& InOutArray);

	FORCEINLINE bool HasKeyWidget() const { return ETB_Key != nullptr; }

	FOnItemRequestRemoved OnItemRequestRemoved;
	
protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnDeleteButtonClicked();
	
	UPROPERTY(Meta = (BindWidgetOptional))
	UEditableTextBox* ETB_Key;

	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_Value;

	UPROPERTY(Meta = (BindWidget))
	UButton* DeleteButton;
};
