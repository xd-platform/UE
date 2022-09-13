#include "XUUserCenterTipWidget.h"

#include "TUSettings.h"
#include "XULanguageManager.h"
#include "XULoginTypeModel.h"

UXUUserCenterTipWidget::UXUUserCenterTipWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}


void UXUUserCenterTipWidget::OnTextBoxValueChange(const FText& Content)
{
	if (RedTipLabel->Visibility == ESlateVisibility::Visible)
	{
		RedTipLabel->SetVisibility(ESlateVisibility::Hidden);
		TextBox->WidgetStyle.BackgroundImageNormal.SetResourceObject(GetGrayTexture());
		TextBox->WidgetStyle.BackgroundImageHovered.SetResourceObject(GetGrayTexture());
		TextBox->WidgetStyle.BackgroundImageFocused.SetResourceObject(GetGrayTexture());
	}

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
				TextBox->WidgetStyle.BackgroundImageNormal.SetResourceObject(GetRedTexture());
				TextBox->WidgetStyle.BackgroundImageHovered.SetResourceObject(GetRedTexture());
				TextBox->WidgetStyle.BackgroundImageFocused.SetResourceObject(GetRedTexture());
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
				TextBox->WidgetStyle.BackgroundImageNormal.SetResourceObject(GetRedTexture());
				TextBox->WidgetStyle.BackgroundImageHovered.SetResourceObject(GetRedTexture());
				TextBox->WidgetStyle.BackgroundImageFocused.SetResourceObject(GetRedTexture());
				
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
		DetailLabel->SetText(FText::FromString(langModel->tds_unbind_delete_content.Replace(TEXT("%s"), *XULoginTypeModel(LoginType).TypeName)));
		GreenButtonLabel->SetText(FText::FromString(langModel->tds_cancel));
		WhiteButtonLabel->SetText(FText::FromString(langModel->tds_delete_account_sure));
		break;
	case UnbindThird:
		TitleLabel->SetText(FText::FromString(langModel->tds_unbind_account_title));
		DetailLabel->SetText(FText::FromString(langModel->tds_unbind_content.Replace(TEXT("%s"), *XULoginTypeModel(LoginType).TypeName)));
		GreenButtonLabel->SetText(FText::FromString(langModel->tds_cancel));
		WhiteButtonLabel->SetText(FText::FromString(langModel->tds_unbind_account));
		break;
	}
	TextBox->SetVisibility(ESlateVisibility::Hidden);
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
		break;
	case UnbindThird:
		TitleLabel->SetText(FText::FromString(langModel->tds_unbind_account));
		DetailLabel->SetText(FText::FromString(langModel->tds_unbind_confirm_Content));
		WhiteButtonLabel->SetText(FText::FromString(langModel->tds_cancel));
		GreenButtonLabel->SetText(FText::FromString(langModel->tds_unbind_account_button));
		break;
	}
	
	
	TextBox->WidgetStyle.BackgroundImageNormal.SetResourceObject(GetGrayTexture());
	TextBox->WidgetStyle.BackgroundImageHovered.SetResourceObject(GetGrayTexture());
	TextBox->WidgetStyle.BackgroundImageFocused.SetResourceObject(GetGrayTexture());

	TextBox->SetText(FText::FromString(TEXT("")));
	RedTipLabel->SetText(FText::FromString(langModel->tds_input_error));

	TextBox->SetVisibility(ESlateVisibility::Visible);
	RedTipLabel->SetVisibility(ESlateVisibility::Hidden);
}

UTexture2D*& UXUUserCenterTipWidget::GetGrayTexture()
{
	if (GrayTexture == nullptr)
	{
		GrayTexture = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/XDGSDK/Images/border_gray.border_gray'"));
	}
	return GrayTexture;
}

UTexture2D*& UXUUserCenterTipWidget::GetRedTexture()
{
	if (RedTexture == nullptr)
	{
		RedTexture = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/XDGSDK/Images/border_red.border_red'"));
	}
	return RedTexture;
}


void UXUUserCenterTipWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FirstStepUpdate();
	GreenButton->OnClicked.AddUniqueDynamic(this, &UXUUserCenterTipWidget::OnGreenBtnClick);
	WhiteButton->OnClicked.AddUniqueDynamic(this, &UXUUserCenterTipWidget::OnWhiteBtnClick);
	TextBox->OnTextChanged.AddUniqueDynamic(this, &UXUUserCenterTipWidget::OnTextBoxValueChange);

	
}
