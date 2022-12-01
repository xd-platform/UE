// Fill out your copyright notice in the Description page of Project Settings.


#include "XDButton.h"

#include "TUDebuger.h"

#if WITH_EDITOR
void UXDButton::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);
	auto PropertyName = PropertyChangedEvent.GetPropertyName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UXDButton, DefaultBrush) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(UXDButton, HighLight)) {
		auto TempBrush = DefaultBrush;
		WidgetStyle.SetNormal(TempBrush);
		float Du = (100 - HighLight) / 100;
		TempBrush.TintColor = FSlateColor(FLinearColor(Du, Du, Du));
		WidgetStyle.SetHovered(TempBrush);
		WidgetStyle.SetPressed(TempBrush);
		WidgetStyle.SetPressedPadding(WidgetStyle.NormalPadding);
		SetStyle(WidgetStyle);
	}
}
#endif

#if WITH_EDITOR

const FText UButton::GetPaletteCategory()
{
	return FText::FromString("XD");
}

#endif

