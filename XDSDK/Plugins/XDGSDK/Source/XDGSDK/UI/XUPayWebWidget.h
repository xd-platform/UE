#pragma once

#include "CoreMinimal.h"
#include "XUType.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "TUWebBrowser.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "XUPayWebWidget.generated.h"


UCLASS()
class XDGSDK_API UXUPayWebWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UXUPayWebWidget(const FObjectInitializer& ObjectInitializer);

	static void Show(const FString& PayUrl, TFunction<void(XUType::PayResult Result)> CallBack);

protected:
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnCloseClick();
	
	UFUNCTION()
	void OnRetryBtnClick();

	UFUNCTION()
	void OnUrlChanged(const FString& Text);

	UFUNCTION()
	void OnWebLoadError();

	

private:
	
	UPROPERTY(meta = (BindWidget))
	UTUWebBrowser* PayWebBrowser;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

	UPROPERTY(meta = (BindWidget))
	UButton* RetryBtn;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RetryBtnLabel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ErrorTipLabel;

	UPROPERTY(meta = (BindWidget))
	UImage* ErrorTipImage;
	
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ErrorBox;

	TFunction<void(XUType::PayResult Result)> CallBack;

	FString PayUrl;

	void CloseWithResult(XUType::PayResult Result);

	void UpdateErrorTipView(bool IsNerworkError);

	void ShowErrorTipView(bool IsShow);
	
};

