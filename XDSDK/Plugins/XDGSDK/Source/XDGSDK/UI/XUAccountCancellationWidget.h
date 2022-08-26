#pragma once

#include "CoreMinimal.h"
#include "XUType.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "TUWebBrowser.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "XUAccountCancellationWidget.generated.h"


UCLASS()
class XDGSDK_API UXUAccountCancellationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UXUAccountCancellationWidget(const FObjectInitializer& ObjectInitializer);

	static void Show(const FString& Url);

protected:
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnCloseClick();

	UFUNCTION()
	void OnBackClick();
	
	UFUNCTION()
	void OnRetryBtnClick();

	UFUNCTION()
	void OnUrlChanged(const FString& Text);

	UFUNCTION()
	void OnTitleChanged(const FString& Text);

	UFUNCTION()
	void OnWebLoadError();

private:
	
	UPROPERTY(meta = (BindWidget))
	UTUWebBrowser* WebBrowser;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseBtn;

	UPROPERTY(meta = (BindWidget))
	UButton* BackBtn;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleLabel;

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
	
	FString Url;

	bool HasCancelAccount = false;
	
	void UpdateErrorTipView(bool IsNerworkError);

	void ShowErrorTipView(bool IsShow);
	
};

