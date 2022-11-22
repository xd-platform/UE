#include "XUUserCenterItemWidget.h"

#include "TUSettings.h"
#include "XUConfigManager.h"
#include "XUServerConfig.h"
#include "XULanguageManager.h"

UXUUserCenterItemWidget::UXUUserCenterItemWidget(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer) {
}

UXUUserCenterItemWidget* UXUUserCenterItemWidget::GenerateItem() {
	UXUUserCenterItemWidget* widget = nullptr;
	if (UClass* MyWidgetClass = LoadClass<UXUUserCenterItemWidget>(
		nullptr, TEXT("WidgetBlueprint'/XDGSDK/BPXUUserCenterItem.BPXUUserCenterItem_C'"))) {
		if (TUSettings::GetGameInstance().IsValid()) {
			widget = CreateWidget<UXUUserCenterItemWidget>(TUSettings::GetGameInstance().Get(), MyWidgetClass);
		}
	}
	return widget;
}


void UXUUserCenterItemWidget::SetBindModel(TSharedPtr<XUUserCenterItemModel> Model) {
	if (!Model.IsValid()) {
		return;
	}
	BindModel = Model;
	auto langModel = XULanguageManager::GetCurrentModel();
	FString Content = langModel->tds_account_format.Replace(TEXT("%s"), *Model->LoginTypeName);
	TitleLabel->SetText(FText::FromString(Content));

	if (Model->LoginType == XUType::TapTap) {
		UTexture2D* TapTexture = LoadObject<UTexture2D>(
			nullptr, TEXT("Texture2D'/XDGSDK/Images/type_icon_tap.type_icon_tap'"));
		TitleImage->SetBrushFromTexture(TapTexture);
	}
	else if (Model->LoginType == XUType::Google) {
		UTexture2D* TapTexture = LoadObject<UTexture2D>(
			nullptr, TEXT("Texture2D'/XDGSDK/Images/type_icon_google.type_icon_google'"));
		TitleImage->SetBrushFromTexture(TapTexture);
	}
	else if (Model->LoginType == XUType::Apple) {
		UTexture2D* TapTexture = LoadObject<UTexture2D>(
			nullptr, TEXT("Texture2D'/XDGSDK/Images/type_icon_apple.type_icon_apple'"));
		TitleImage->SetBrushFromTexture(TapTexture);
	}
	else if (Model->LoginType == XUType::Steam) {
		UTexture2D* TapTexture = LoadObject<UTexture2D>(
			nullptr, TEXT("Texture2D'/XDGSDK/Images/type_icon_steam.type_icon_steam'"));
		TitleImage->SetBrushFromTexture(TapTexture);
	}
	if (Model->BindState == FXDGBindState::Bind) {
		BindLabel->SetText(FText::FromString(langModel->tds_unbind));
		BindLabel->SetColorAndOpacity(FLinearColor(0.6f, 0.6f, 0.6f, 1));
		ArrowImage->SetBrushFromTexture(
			LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/XDGSDK/Images/arrow_gray.arrow_gray'")));
		ShowBindBt(Model->CanUnbind);
	}
	else {
		BindLabel->SetText(FText::FromString(langModel->tds_bind));
		BindLabel->SetColorAndOpacity(FLinearColor::Black);
		ArrowImage->SetBrushFromTexture(
			LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/XDGSDK/Images/arrow_black.arrow_black'")));
		ShowBindBt(Model->CanBind);
	}
}

void UXUUserCenterItemWidget::NativeConstruct() {
	Super::NativeConstruct();
	BindButton->OnClicked.AddUniqueDynamic(this, &UXUUserCenterItemWidget::OnBindBtnClick);
}

void UXUUserCenterItemWidget::OnBindBtnClick() {
	if (BindCallBack) {
		BindCallBack(this, BindModel);
	}
}

void UXUUserCenterItemWidget::ShowBindBt(int Show) {
	if (Show == 1) {
		BindButton->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		BindButton->SetVisibility(ESlateVisibility::Hidden);
	}
}
