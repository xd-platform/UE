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
	Default	= 0,	// 自动登录
	Guest = 1,		// 游客登录
	// WeChat = 2,		
	// Apple = 3,		
	Google = 4,		
	// Facebook = 5,		
	TapTap = 6,		// TapTap登录
	// Line = 7,
	// Twitter = 8
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
	UEditableTextContainerWidget* ET_Login_LoginTypes;
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
	UEditableTextBox* ETB_Bind_LoginType;
	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* BindByType;
};
