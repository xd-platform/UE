// Fill out your copyright notice in the Description page of Project Settings.


#include "ServiceItemWidget.h"

#include "Components/TextBlock.h"


void UServiceItemWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	ButtonText->SetText(ButtonDisplayText);
}
