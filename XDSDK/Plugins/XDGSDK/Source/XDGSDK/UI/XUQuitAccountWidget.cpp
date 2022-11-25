// Fill out your copyright notice in the Description page of Project Settings.


#include "XUQuitAccountWidget.h"

#include "Components/Button.h"

void UXUQuitAccountWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ConfirmButton->OnClicked.AddDynamic(this, &UXUQuitAccountWidget::OnConfirmClicked);
	CancelButton->OnClicked.AddDynamic(this, &UXUQuitAccountWidget::OnCancelClicked);
	CloseButton->OnClicked.AddDynamic(this, &UXUQuitAccountWidget::RemoveFromParent);
}

void UXUQuitAccountWidget::OnConfirmClicked()
{
	OnConfirm.ExecuteIfBound();
}

void UXUQuitAccountWidget::OnCancelClicked()
{
	OnCancel.ExecuteIfBound();
}
