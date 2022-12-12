// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TapWebBrowser.h"
#include "Components/RichTextBlock.h"
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

	UFUNCTION()
	void OnKrPushCheckProtocolButtonClick();

	virtual void Reload() override;

	virtual void OnLoadStarted() override;

	virtual void OnLoadCompleted() override;

	virtual void OnLoadError() override;

	virtual bool OnBeforeNavigation(const FString& URL, const FWebNavigationRequest& Request) override;

	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
	
private:
	UPROPERTY(meta = (BindWidget))
	UButton* ComfirmButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ComfirmButtonLabel;

	// UPROPERTY(meta = (BindWidget))
	// UImage* ComfirmButtonImage;

	UPROPERTY(meta = (BindWidget))
	UButton* DeclineButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DeclineButtonLabel;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* AmericaCheckBox;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmericaCheckLabel;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* KrAgeCheckBox;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* KrAgeCheckLabel;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* KrAgeCheckTagLabel;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* KrPushCheckBox;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* KrPushCheckTagLabel;
	UPROPERTY(meta = (BindWidget))
	UButton* KrPushCheckProtocolButton;
	UPROPERTY(meta = (BindWidget))
	URichTextBlock* KrPushCheckLabel;
	
	UPROPERTY(EditDefaultsOnly)
	FSlateBrush ComfirmBrush;

	UPROPERTY(EditDefaultsOnly)
	FSlateBrush ComfirmDisableBrush;

	UPROPERTY(EditDefaultsOnly)
	FSlateBrush ComfirmHoverBrush;
	
	bool IsInKrAndPushEnable();

	bool IsInNorthAmerica();

	void UpdateComfirmBtnState();

	TFunction<void()> Completed;
	
	FString OriginURL;

	FString NavigationUrl;
};
