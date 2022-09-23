#pragma once

#include "CoreMinimal.h"
#include "TapWebBrowser.h"
#include "XUType.h"
#include "XUPayWebWidget.generated.h"


UCLASS()
class XDGSDK_API UXUPayWebWidget : public UTapWebBrowser
{
	GENERATED_BODY()

public:
	static void Show(const FString& PayUrl, TFunction<void(XUType::PayResult Result)> CallBack);

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	virtual void Close() override;

	virtual void Reload() override;

	virtual void OnURLChanged(const FText& NewURL) override;

	virtual void OnLoadStarted() override;
	
	virtual void OnLoadCompleted() override;

	virtual void OnLoadError() override;
	
private:
	TFunction<void(XUType::PayResult Result)> CallBack;

	FString PayUrl;

	void CloseWithResult(XUType::PayResult Result);

	void UpdateErrorTipView(bool IsNerworkError);

	void ShowErrorTipView(bool IsShow);
	
};

