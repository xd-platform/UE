// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TUWebBrowser.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "XUPrivacyWidget.generated.h"

/**
 *  
 */
UCLASS()
class XDGSDK_API UXUPrivacyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	 UXUPrivacyWidget(const FObjectInitializer& ObjectInitializer);

	static void ShowPrivacy(TFunction<void()> Completed);

protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnCheckStateChanged(bool isChecked);

	UFUNCTION()
	void OnComfirmBtnClick();

	UFUNCTION()
	void OnLoadErrorBtnClick();

	UFUNCTION()
	void OnDeclineBtnClick();

	UFUNCTION()
	void OnWebLoadCompleted();

	UFUNCTION()
	void OnWebLoadError();
	
	bool OnWebBeforeNavigation(const FString& Url, const FWebNavigationRequest& Request);
	
private:

	UPROPERTY(meta = (BindWidget))
	UTUWebBrowser* PrivacyWebBrowser;

	UPROPERTY(meta = (BindWidget))
	UButton* ComfirmButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ComfirmButtonLabel;

	UPROPERTY(meta = (BindWidget))
	UButton* LoadErrorBtn;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LoadErrorLabel;

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
	
	TFunction<void()> Completed;

	bool IsInKrAndPushEnable();

	bool IsInNorthAmerica();

	void UpdateComfirmBtnState();

	FString OriginURL;

	enum LoadState {
		Loading,
		LoadError,
		LoadSuccess,
	};

	void UpdateUI(LoadState State);

	FString NavigationUrl;
};
