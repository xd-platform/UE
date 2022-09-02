// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EditableTextContainerWidget.generated.h"

class UTextBlock;
class UButton;
class UEditableTextItemWidget;
/**
 * 
 */
UCLASS(Abstract)
class XDSDK_API UEditableTextContainerWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void RemoveChild(UEditableTextItemWidget* Child);

	TSharedPtr<FJsonObject> MakeJsonObject() const;

	TArray<FString> MakeStringArray() const;

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnAddButtonClicked();

	UFUNCTION()
	void OnRemoveAllButtonClicked();
	
	void UpdateChildrenNumber();
	
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* TB_ElementNumber;

	UPROPERTY(Meta = (BindWidget))
	UButton* BTN_Add;

	UPROPERTY(Meta = (BindWidget))
	UButton* BTN_RemoveAll;

	UPROPERTY(Meta = (BindWidget))
	UPanelWidget* Panel;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UEditableTextItemWidget> ItemClass;
};
