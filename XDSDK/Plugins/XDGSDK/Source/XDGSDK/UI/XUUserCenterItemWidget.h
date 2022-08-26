#pragma once

#include "CoreMinimal.h"
#include "XUBindModel.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "XUUserCenterItemWidget.generated.h"


UCLASS()
class XDGSDK_API UXUUserCenterItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UXUUserCenterItemWidget(const FObjectInitializer& ObjectInitializer);

	static UXUUserCenterItemWidget * GenerateItem();

	void SetBindModel(const TSharedPtr<FXUBindModel>& Model);

	TFunction<void(UXUUserCenterItemWidget *CurrentWidget, TSharedPtr<FXUBindModel> Model)> BindCallBack;

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
	UImage* ArrowImage;

	TSharedPtr<FXUBindModel> BindModel;
	// TFunction<void(bool result)> Completed;

	void ProcessShowOrNot();

	void ShowBindBt(int Show);
	
	
};