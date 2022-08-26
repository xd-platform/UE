#pragma once

#include "CoreMinimal.h"
#include "XUType.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "XUPayHintAlert.generated.h"


UCLASS()
class XDGSDK_API UXUPayHintAlert : public UUserWidget
{
	GENERATED_BODY()

public:
	UXUPayHintAlert(const FObjectInitializer& ObjectInitializer);

	static void Show(XUType::CheckPayType CheckType);

protected:
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnContactCustomerServiceBtnClick();

private:
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleLabel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DetailLabel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ContentLabel;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TailLabel;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ButtonLabel;

	UPROPERTY(meta = (BindWidget))
	UButton* ContactCustomerServiceButton;

	XUType::CheckPayType CheckType = XUType::None;
	
};

