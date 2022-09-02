// Fill out your copyright notice in the Description page of Project Settings.


#include "EditableTextContainerWidget.h"

#include "Components/Button.h"
#include "EditableTextItemWidget.h"
#include "Components/TextBlock.h"

void UEditableTextContainerWidget::RemoveChild(UEditableTextItemWidget* Child)
{
	Panel->RemoveChild(Child);
	UpdateChildrenNumber();
}

TSharedPtr<FJsonObject> UEditableTextContainerWidget::MakeJsonObject() const
{
	TSharedPtr<FJsonObject> Obj = MakeShared<FJsonObject>();
	for (int32 i = 0; i < Panel->GetChildrenCount(); ++i)
	{
		if (UEditableTextItemWidget* ChildUI = Cast<UEditableTextItemWidget>(Panel->GetChildAt(i)))
		{
			if (!ensureMsgf(ChildUI->HasKeyWidget(), TEXT("Must has key widget binding, if you want to make JsonObject.")))
			{
				return nullptr;
			}
			ChildUI->AppendJsonObject(Obj);
		}
		else
		{
			ensure(false);
		}
	}
	return Obj->Values.Num() > 0 ? Obj : nullptr;
}

TArray<FString> UEditableTextContainerWidget::MakeStringArray() const
{
	TArray<FString> Results;
	for (int32 i = 0; i < Panel->GetChildrenCount(); ++i)
	{
		if (UEditableTextItemWidget* ChildUI = Cast<UEditableTextItemWidget>(Panel->GetChildAt(i)))
		{
			ensureMsgf(!ChildUI->HasKeyWidget(), TEXT("No need to use key binding widget, if you want to get string array."));
			ChildUI->AppendStringArray(Results);
		}
		else
		{
			ensure(false);
		}
	}
	return Results;
}

void UEditableTextContainerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BTN_Add->OnClicked.AddDynamic(this, &UEditableTextContainerWidget::OnAddButtonClicked);
	BTN_RemoveAll->OnClicked.AddDynamic(this, &UEditableTextContainerWidget::OnRemoveAllButtonClicked);
	
	UpdateChildrenNumber();
}

void UEditableTextContainerWidget::OnAddButtonClicked()
{
	if (UEditableTextItemWidget* NewUI = CreateWidget<UEditableTextItemWidget>(this, ItemClass))
	{
		NewUI->OnItemRequestRemoved.BindUObject(this, &UEditableTextContainerWidget::RemoveChild);
		Panel->AddChild(NewUI);
		UpdateChildrenNumber();
	}
}

void UEditableTextContainerWidget::OnRemoveAllButtonClicked()
{
	Panel->ClearChildren();
	UpdateChildrenNumber();
}

void UEditableTextContainerWidget::UpdateChildrenNumber()
{
	const FText NewNumberText = FText::Format(NSLOCTEXT("TapDemo", "MapWidgetElementNumberFormat", "{0} Elements"),
		FText::AsNumber(Panel->GetChildrenCount(), &FNumberFormattingOptions::DefaultNoGrouping()));
	TB_ElementNumber->SetText(NewNumberText);
}

