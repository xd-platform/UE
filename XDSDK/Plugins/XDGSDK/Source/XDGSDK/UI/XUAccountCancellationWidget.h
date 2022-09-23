#pragma once

#include "CoreMinimal.h"
#include "TapWebBrowser.h"
#include "Components/VerticalBox.h"
#include "XUAccountCancellationWidget.generated.h"


UCLASS()
class XDGSDK_API UXUAccountCancellationWidget : public UTapWebBrowser
{
	GENERATED_BODY()

public:
	static void Show(const FString& Url);

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	virtual void GoBack() override;
	
	virtual void Close() override;

	virtual void Reload() override;

	virtual void OnURLChanged(const FText& NewURL) override;

	virtual void OnTitleChanged(const FText& NewTitle) override;

	virtual void OnLoadError() override;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleLabel;
	
	FString Url;

	bool HasCancelAccount = false;
	
	void UpdateErrorTipView(bool IsNerworkError);

	void ShowErrorTipView(bool IsShow);
	
};

