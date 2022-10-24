// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "XDServiceWidgetAccount.generated.h"

class UComboBoxString;
class UEditableTextBox;
class UEditableTextContainerWidget;
class UServiceItemWidget;
UENUM()
enum class ETempDemoLoginType : uint8
{
	Guest = 0,		// 游客登录
	WeChat = 1,		
	Apple = 2,		
	Google = 3,		
	Facebook = 4,		
	TapTap = 5,		// TapTap登录
	Line = 6,
	Twitter = 7,
	Default	= 100,	// 自动登录
};

/**
 * 
 */
UCLASS(Abstract)
class XDSDK_API UXDServiceWidgetAccount : public UUserWidget
{
	GENERATED_BODY()

	UFUNCTION()
	void OnLoginClicked();
	
	UFUNCTION()
	void OnLoginByTypeClicked();

	UFUNCTION()
	void OnAddUserStatusChangeCallbackClicked();

	UFUNCTION()
	void OnGetUserClicked();
	
	UFUNCTION()
	void OnOpenUserCenterClicked();

	UFUNCTION()
	void OnLogoutClicked();

	UFUNCTION()
	void OnOpenAccountCancellationClicked();
	
	UFUNCTION()
	void OnIsTokenActiveWithTypeClicked();

	UFUNCTION()
	void OnBindByTypeClicked();
	
protected:
	virtual void NativeOnInitialized() override;

	void OnXDGSDKLoginSucceed(const FString&);

	void OnXDGSDKLoginFailed(const int32, const FString&, const FString&);

	void OnXDGSDKGetUserSucceed(const FString&);

	void OnXDGSDKGetUserFailed(const int32, const FString&);

	void OnXDGSDKUserStateChanged(const int32, const FString&);

	void OnXDGSDKBindByTypeCompleted(const bool, const int32, const FString&);


	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* Login;

	UPROPERTY(Meta = (BindWidget))
	UComboBoxString* CB_LoginByType_LoginType;
	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* LoginByType;

	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* AddUserStatusChangeCallback;

	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* GetUser;
	
	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* OpenUserCenter;

	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* Logout;

	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* OpenAccountCancellation;

	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_Token_LoginType;
	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* IsTokenActiveWithType;
	
	UPROPERTY(Meta = (BindWidget))
	UComboBoxString* CB_Bind_LoginType;
	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* BindByType;
};
