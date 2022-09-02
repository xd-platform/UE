// Fill out your copyright notice in the Description page of Project Settings.


#include "EditableTextItemWidget.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"

void UEditableTextItemWidget::AppendJsonObject(const TSharedPtr<FJsonObject>& InOutJsonObject)
{
	check(InOutJsonObject);
	checkf(ETB_Key, TEXT("Must has key widget binding, if you want to make JsonObject."));
	const FString KeyStr = ETB_Key->GetText().ToString();
	const FString ValueStr = ETB_Value->GetText().ToString();
	InOutJsonObject->SetStringField(KeyStr, ValueStr);
}

void UEditableTextItemWidget::AppendStringArray(TArray<FString>& InOutArray)
{
	const FString ValueStr = ETB_Value->GetText().ToString();
	InOutArray.Add(ValueStr);
}

void UEditableTextItemWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	DeleteButton->OnClicked.AddDynamic(this, &UEditableTextItemWidget::OnDeleteButtonClicked);
}

void UEditableTextItemWidget::OnDeleteButtonClicked()
{
	if (OnItemRequestRemoved.IsBound())
	{
		OnItemRequestRemoved.Execute(this);
	}
}
