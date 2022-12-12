
#pragma once

#include "CoreMinimal.h"
#include "XUType.h"

#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "XUUserCenterTipWidget.generated.h"


class URichTextBlock;
class UBorder;
UCLASS()
class XDGSDK_API UXUUserCenterTipWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	enum AlertType
	{
		DeleteGuest = 0,   //删除游客
		DeleteThird = 1,   //删除第三方
		UnbindThird = 2,   //解绑第三方
	};
	UXUUserCenterTipWidget(const FObjectInitializer& ObjectInitializer);

	static void Show(AlertType AlertType, XUType::LoginType LoginType, TFunction<void()> SureCallBack, TFunction<void()> CancelCallBack);


protected:
	virtual void NativeOnInitialized() override;
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnGreenBtnClick();

	UFUNCTION()
	void OnWhiteBtnClick();

	UFUNCTION()
	void OnTextBoxValueChange(const FText& Content);

	void ShowRedBorder(bool bShow);


private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleLabel;

	UPROPERTY(meta = (BindWidget))
	URichTextBlock* DetailLabel;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* TextBox;

	UPROPERTY(meta = (BindWidget))
	UBorder* TextBorder;

	UPROPERTY(meta = (BindWidget))
	UButton* GreenButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GreenButtonLabel;

	UPROPERTY(meta = (BindWidget))
	UButton* WhiteButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WhiteButtonLabel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RedTipLabel;


	AlertType AlertType;
	XUType::LoginType LoginType;
	TFunction<void()> SureCallBack;
	TFunction<void()> CancelCallBack;

	void FirstStepUpdate();
	void SecondStepUpdate();

	enum Step
	{
		First,
		Second,
	};

	Step Step;
};