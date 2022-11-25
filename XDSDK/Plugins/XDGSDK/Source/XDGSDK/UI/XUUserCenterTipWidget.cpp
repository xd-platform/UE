#include "XUUserCenterTipWidget.h"

#include "TUSettings.h"
#include "XULanguageManager.h"
#include "XULoginTypeModel.h"
#include "Components/Border.h"
#include "Components/RichTextBlock.h"

UXUUserCenterTipWidget::UXUUserCenterTipWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}


void UXUUserCenterTipWidget::OnTextBoxValueChange(const FText& Content)
{
	if (RedTipLabel->Visibility == ESlateVisibility::Visible)
	{
		RedTipLabel->SetVisibility(ESlateVisibility::Hidden);
		ShowRedBorder(false);
	}
}

void UXUUserCenterTipWidget::ShowRedBorder(bool bShow)
{
	TextBorder->SetBrushColor(bShow ? FLinearColor(0.921582f, 0.072272f, 0.072272f) : FLinearColor(0.f, 0.f, 0.f, 0.078431f));
}

void UXUUserCenterTipWidget::Show(enum AlertType AlertType, ::XUType::LoginType LoginType, TFunction<void()> SureCallBack,
                                  TFunction<void()> CancelCallBack)
{
	if (UClass* MyWidgetClass = LoadClass<UXUUserCenterTipWidget>(nullptr, TEXT("WidgetBlueprint'/XDGSDK/BPXUUserCenterTIp.BPXUUserCenterTIp_C'")))
	{
		if (TUSettings::GetGameInstance().IsValid()) {
			auto widget = CreateWidget<UXUUserCenterTipWidget>(TUSettings::GetGameInstance().Get(), MyWidgetClass);
			widget->AlertType = AlertType;
			widget->LoginType = LoginType;
			widget->SureCallBack = SureCallBack;
			widget->CancelCallBack = CancelCallBack;
			widget->AddToViewport(TUSettings::GetUILevel());
		}
	}
}

void UXUUserCenterTipWidget::OnGreenBtnClick()
{
	if (Step == First)
	{
		if (CancelCallBack)
		{
			CancelCallBack();
		}
		RemoveFromParent();
	} else if (Step == Second)
	{
		FString Content = TextBox->Text.ToString();
		if (AlertType == DeleteGuest || AlertType == DeleteThird)
		{
			if (Content.Equals(TEXT("Delete")))
			{
				if (SureCallBack)
				{
					SureCallBack();
				}
				RemoveFromParent();
			} else
			{
				RedTipLabel->SetVisibility(ESlateVisibility::Visible);
				ShowRedBorder(true);
			}
		} else
		{
			if (Content.Equals(TEXT("Confirm")))
			{
				if (SureCallBack)
				{
					SureCallBack();
				}
				RemoveFromParent();
			} else
			{
				RedTipLabel->SetVisibility(ESlateVisibility::Visible);
				ShowRedBorder(true);
			}
		}
	}
}

void UXUUserCenterTipWidget::OnWhiteBtnClick()
{
	if (Step == First)
	{
		SecondStepUpdate();
	} else if (Step == Second)
	{
		if (CancelCallBack)
		{
			CancelCallBack();
		}
		RemoveFromParent();
	}
}


void UXUUserCenterTipWidget::FirstStepUpdate()
{
	Step = First;
	auto langModel = XULanguageManager::GetCurrentModel();
	switch (AlertType)
	{
	case DeleteGuest:
		TitleLabel->SetText(FText::FromString(langModel->tds_delete_account_title));
		DetailLabel->SetText(FText::FromString(langModel->tds_delete_content));
		GreenButtonLabel->SetText(FText::FromString(langModel->tds_cancel));
		WhiteButtonLabel->SetText(FText::FromString(langModel->tds_delete_account_sure));
		break;
	case DeleteThird:
		TitleLabel->SetText(FText::FromString(langModel->tds_delete_account_title));
		DetailLabel->SetText(FText::FromString(langModel->tds_unbind_delete_content.Replace(TEXT("%s"), *FString::Printf(TEXT("<Account>%s</>"), *XULoginTypeModel(LoginType).TypeName))));
		GreenButtonLabel->SetText(FText::FromString(langModel->tds_cancel));
		WhiteButtonLabel->SetText(FText::FromString(langModel->tds_delete_account_sure));
		break;
	case UnbindThird:
		TitleLabel->SetText(FText::FromString(langModel->tds_unbind_account_title));
		DetailLabel->SetText(FText::FromString(langModel->tds_unbind_content.Replace(TEXT("%s"), *FString::Printf(TEXT("<Account>%s</>"), *XULoginTypeModel(LoginType).TypeName))));
		GreenButtonLabel->SetText(FText::FromString(langModel->tds_cancel));
		WhiteButtonLabel->SetText(FText::FromString(langModel->tds_unbind_account));
		break;
	}
	TextBorder->SetVisibility(ESlateVisibility::Collapsed);
	RedTipLabel->SetVisibility(ESlateVisibility::Collapsed);
}

void UXUUserCenterTipWidget::SecondStepUpdate()
{
	Step = Second;
	auto langModel = XULanguageManager::GetCurrentModel();
	switch (AlertType)
	{
	case DeleteGuest:
		TitleLabel->SetText(FText::FromString(langModel->tds_delete_account));
		DetailLabel->SetText(FText::FromString(langModel->tds_delete_confirm_content));
		WhiteButtonLabel->SetText(FText::FromString(langModel->tds_cancel));
		GreenButtonLabel->SetText(FText::FromString(langModel->tds_delete_account));
		break;
	case DeleteThird:
		TitleLabel->SetText(FText::FromString(langModel->tds_delete_account));
		DetailLabel->SetText(FText::FromString(langModel->tds_delete_confirm_content));
		WhiteButtonLabel->SetText(FText::FromString(langModel->tds_cancel));
		GreenButtonLabel->SetText(FText::FromString(langModel->tds_delete_account));
		TextBox->SetHintText(NSLOCTEXT("FXDGSDKModule", "DeleteAccount", "Delete"));
		break;
	case UnbindThird:
		TitleLabel->SetText(FText::FromString(langModel->tds_unbind_account));
		DetailLabel->SetText(FText::FromString(langModel->tds_unbind_confirm_Content));
		WhiteButtonLabel->SetText(FText::FromString(langModel->tds_cancel));
		GreenButtonLabel->SetText(FText::FromString(langModel->tds_unbind_account_button));
		TextBox->SetHintText(NSLOCTEXT("FXDGSDKModule", "ConfirmUnbindAccount", "Confirm"));
		break;
	}
	
	
	ShowRedBorder(false);

	TextBox->SetText(FText::FromString(TEXT("")));
	RedTipLabel->SetText(FText::FromString(langModel->tds_input_error));

	TextBorder->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	RedTipLabel->SetVisibility(ESlateVisibility::Hidden);
}

void UXUUserCenterTipWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	CloseButton->OnClicked.AddDynamic(this, &UXUUserCenterTipWidget::RemoveFromParent);
}

void UXUUserCenterTipWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FirstStepUpdate();
	GreenButton->OnClicked.AddUniqueDynamic(this, &UXUUserCenterTipWidget::OnGreenBtnClick);
	WhiteButton->OnClicked.AddUniqueDynamic(this, &UXUUserCenterTipWidget::OnWhiteBtnClick);
	TextBox->OnTextChanged.AddUniqueDynamic(this, &UXUUserCenterTipWidget::OnTextBoxValueChange);

	
}
