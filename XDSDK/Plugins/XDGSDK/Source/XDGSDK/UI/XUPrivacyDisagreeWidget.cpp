// Fill out your copyright notice in the Description page of Project Settings.


#include "XUPrivacyDisagreeWidget.h"

#include "XULanguageManager.h"
#include "TUHelper.h"
#include "TUHUD.h"
#include "TUSettings.h"
#include "XUConfigManager.h"


UXUPrivacyDisagreeWidget::UXUPrivacyDisagreeWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UXUPrivacyDisagreeWidget::Show()
{
	if (UClass* MyWidgetClass = LoadClass<UXUPrivacyDisagreeWidget>(nullptr, TEXT("WidgetBlueprint'/XDGSDK/BPPrivacyDisagreeUI.BPPrivacyDisagreeUI_C'")))
	{
		if (TUSettings::GetGameInstance().IsValid()) {
			auto widget = CreateWidget<UXUPrivacyDisagreeWidget>(TUSettings::GetGameInstance().Get(), MyWidgetClass);
			widget->AddToViewport(TUSettings::GetUILevel());
		}
	}
}

void UXUPrivacyDisagreeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	FScriptDelegate BtnDel;
	BtnDel.BindUFunction(this, "OnComfirmBtnClick");
	ComfirmButton->OnClicked.Add(BtnDel);
	DeclineButton->OnClicked.AddUniqueDynamic(this, &UXUPrivacyDisagreeWidget::OnDeclineBtnClick);
	
	
	auto langModel = XULanguageManager::GetCurrentModel();
	ComfirmButtonLabel->SetText(FText::FromString(langModel->xd_agreement_disagree_confirm_back));
	DeclineButtonLabel->SetText(FText::FromString(langModel->xd_agreement_disagree_confirm_exit));
	ContentLabel->SetText(FText::FromString(langModel->xd_agreement_disagree_confirm_content));
	
}

void UXUPrivacyDisagreeWidget::OnComfirmBtnClick() {
	RemoveFromParent();
}

void UXUPrivacyDisagreeWidget::OnDeclineBtnClick() {
#if WITH_EDITOR
	GEngine->Exec(nullptr, TEXT("QUIT_EDITOR"), *GLog);
#else
	FGenericPlatformMisc::RequestExit(false);
#endif
}





