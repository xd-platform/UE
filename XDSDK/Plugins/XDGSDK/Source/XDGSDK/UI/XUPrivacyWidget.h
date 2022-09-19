// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TapWebBrowser.h"
#include "XUPrivacyWidget.generated.h"

class UCheckBox;
class UNativeWidgetHost;
/**
 *  
 */
UCLASS()
class XDGSDK_API UXUPrivacyWidget : public UTapWebBrowser
{
	GENERATED_BODY()
public:
	static void ShowPrivacy(TFunction<void()> Completed);

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnCheckStateChanged(bool isChecked);

	UFUNCTION()
	void OnConfirmBtnClick();
	
	UFUNCTION()
	void OnDeclineBtnClick();

	virtual void Reload() override;

	virtual void OnLoadStarted() override;

	virtual void OnLoadCompleted() override;

	virtual void OnLoadError() override;

	virtual bool OnBeforeNavigation(const FString& URL, const FWebNavigationRequest& Request) override;
	
private:
	UPROPERTY(meta = (BindWidget))
	UButton* ComfirmButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ComfirmButtonLabel;

	UPROPERTY(meta = (BindWidget))
	UImage* ComfirmButtonImage;

	UPROPERTY(meta = (BindWidget))
	UButton* DeclineButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DeclineButtonLabel;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* AdditionalCheckBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AdditionalCheckLabel;
	

	bool IsInKrAndPushEnable();

	bool IsInNorthAmerica();

	void UpdateComfirmBtnState();

	TFunction<void()> Completed;
	
	FString OriginURL;

	FString NavigationUrl;
};
