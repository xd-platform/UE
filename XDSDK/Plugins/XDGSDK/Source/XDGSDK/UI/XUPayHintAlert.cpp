#include "XUPayHintAlert.h"

#include "TUSettings.h"
#include "XULanguageManager.h"
#include "XDUE.h"

UXUPayHintAlert::UXUPayHintAlert(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UXUPayHintAlert::Show(XUType::CheckPayType CheckType)
{
	if (CheckType == XUType::None)
	{
		return;
	}
	if (UClass* MyWidgetClass = LoadClass<UXUPayHintAlert>(nullptr, TEXT("WidgetBlueprint'/XDGSDK/BPXUPayHintAlert.BPXUPayHintAlert_C'")))
	{
		if (TUSettings::GetGameInstance().IsValid()) {
			auto Widget = CreateWidget<UXUPayHintAlert>(TUSettings::GetGameInstance().Get(), MyWidgetClass);
			Widget->CheckType = CheckType;
			Widget->AddToViewport(TUSettings::GetUILevel());
		}
	}
}

void UXUPayHintAlert::NativeConstruct()
{
	Super::NativeConstruct();
	auto langModel = XULanguageManager::GetCurrentModel();
	TitleLabel->SetText(FText::FromString(langModel->tds_refund_login_restrict_title));
	DetailLabel->SetText(FText::FromString(langModel->tds_refund_login_restrict_sub_title));
	TailLabel->SetText(FText::FromString(langModel->tds_refund_custom_service_tip));
	ButtonLabel->SetText(FText::FromString(langModel->tds_refund_contact_custom_service));

	if (CheckType == XUType::iOSAndAndroid)
	{
		ContentLabel->SetText(FText::FromString(langModel->tds_refund_all_pay_tip));
	} else if (CheckType == XUType::iOS)
	{
		ContentLabel->SetText(FText::FromString(langModel->tds_refund_ios_pay_tip));
	} else if (CheckType == XUType::Android)
	{
		ContentLabel->SetText(FText::FromString(langModel->tds_refund_android_pay_tip));
	} else
	{
		ContentLabel->SetText(FText::FromString(""));
	}

	
	ContactCustomerServiceButton->OnClicked.AddUniqueDynamic(this, &UXUPayHintAlert::OnContactCustomerServiceBtnClick);
}

void UXUPayHintAlert::OnContactCustomerServiceBtnClick()
{
	XDUE::OpenCustomerCenter("", "", "");
}
