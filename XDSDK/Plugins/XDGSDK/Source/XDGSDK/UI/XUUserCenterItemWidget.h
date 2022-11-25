#pragma once

#include "CoreMinimal.h"
#include "TULoginType.h"
#include "XUBindModel.h"
#include "XUType.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "XUUserCenterItemWidget.generated.h"

class XUUserCenterItemModel {
public:
	XUType::LoginType LoginType = XUType::Default;
	FString LoginTypeName = "";
	bool CanBind = true;
	bool CanUnbind = true;
	FXDGBindState BindState = UnBind;
};


UCLASS()
class XDGSDK_API UXUUserCenterItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UXUUserCenterItemWidget(const FObjectInitializer& ObjectInitializer);

	static UXUUserCenterItemWidget * GenerateItem();

	void SetBindModel(TSharedPtr<XUUserCenterItemModel> Model);

	void SetDividingLineVisible(bool bNewVisible);

	TFunction<void(UXUUserCenterItemWidget *CurrentWidget, TSharedPtr<XUUserCenterItemModel> Model)> BindCallBack;

protected:

	virtual void NativeConstruct() override;


	UFUNCTION()
	void OnBindBtnClick();


private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleLabel;

	UPROPERTY(meta = (BindWidget))
	UImage* TitleImage;

	UPROPERTY(meta = (BindWidget))
	UButton* BindButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* BindLabel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* IsBindingLabel;

	UPROPERTY(meta = (BindWidget))
	UImage* Line;

	TSharedPtr<XUUserCenterItemModel> BindModel;

	void ShowBindBt(int Show);
	
	
};