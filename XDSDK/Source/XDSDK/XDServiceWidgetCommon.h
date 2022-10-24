// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "XDServiceWidgetCommon.generated.h"

class UCheckBox;
class UEditableTextBox;
class UComboBoxString;
class UServiceItemWidget;

UENUM()
enum class ETempDemoCountryType : uint8
{
	/** Default */
	DF,
	/** Hong Kong */
	HK,
	/** America */
	US,
	/** Korea */
	KR,
	/** Japan */
	JP
};

UENUM()
enum class ETempDemoEnvironmentType : uint8
{
	/** 正式环境 */
	Default,
	/** RND环境 */
	RND
};

UENUM()
enum class ETempDemoRegionType : uint8
{
	/** 国内 */
	CN,
	/** 海外 */
	Global
};



/**
 * 
 */
UCLASS(Abstract)
class XDSDK_API UXDServiceWidgetCommon : public UUserWidget
{
	GENERATED_BODY()
	
	UFUNCTION()
	void OnGetSDKVersionNameClicked();

	// 0 简体中文
	// 1 繁体中文
	// 2 英文
	// 3 泰文
	// 4 印尼文
	// 5 韩文
	// 6 日文
	// 7 德语
	// 8 法语
	// 9 葡萄牙语
	// 10 西班牙语
	// 11 土耳其语
	// 12 俄语
	// 13 越南语
	UFUNCTION()
	void OnSetLanguageClicked();

	UFUNCTION()
	void OnInitSDKClicked();

	UFUNCTION()
	void OnIsInitializedClicked();

	UFUNCTION()
	void OnGetAgreementBeansClicked();

	UFUNCTION()
	void OnOpenAgreementBeanClicked();
	
	UFUNCTION()
	void OnReportClicked();
	
	UFUNCTION()
	void OnStoreReviewClicked();

	UFUNCTION()
	void OnTrackUserClicked();

	UFUNCTION()
	void OnTrackRoleClicked();

	UFUNCTION()
	void OnTrackEventClicked();
	
	UFUNCTION()
	void OnSetCurrentUserPushServiceEnableClicked();
	
	UFUNCTION()
	void OnIsCurrentUserPushServiceEnableClicked();

	UFUNCTION()
	void OnGetRegionInfoClicked();

	UFUNCTION()
	void OnGetXDGInfoJsonClicked();

	UFUNCTION()
	void OnDisableAgreementUIClicked();

	UFUNCTION()
	void OnSetCountryRegionClicked();

	UFUNCTION()
	void OnDevelopInitClicked();
	
	UFUNCTION()
	void OnTrackAchievementClicked();
	
	UFUNCTION()
	void OnEventCompletedTutorialClicked();

	UFUNCTION()
	void OnEventCreateRoleClicked();

	UFUNCTION()
	void OnResetPrivacyClicked();

	UFUNCTION()
	void OnOpenMomentClicked();

	UFUNCTION()
	void OnOpenWebTopicClicked();
	
protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnEnvironmentSelectChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	
	void OnXDGSDKInitCompleted(const bool, const FString&);

	void OnXDGSDKGetXDGInfoJsonCompleted(const FString&);

	//0成功，1取消，2失败
	void OnXDGSDKShareCompleted(const int32);

	void OnXDGSDKGetRegionInfoCompleted(const FString&, const FString&, const FString&, const FString&);

	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* GetSDKVersionName;

	UPROPERTY(Meta = (BindWidget))
	UComboBoxString* CB_Language;
	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* SetLanguage;

	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* InitSDK;

	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* IsInitialized;

	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* GetAgreementBeans;

	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_AgreementBean_Url;
	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* OpenAgreementBean;

	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_Report_ServerID;
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_Report_RoleID;
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_Report_RoleName;
	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* Report;
	
	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* StoreReview;

	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_User_UserID;
	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* TrackUser;

	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_Role_ServerID;
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_Role_RoleID;
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_Role_RoleName;
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_Role_Level;
	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* TrackRole;

	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_Event_EventName;
	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* TrackEvent;

	UPROPERTY(Meta = (BindWidget))
	UCheckBox* CheckBox;
	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* SetCurrentUserPushServiceEnable;
	
	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* IsCurrentUserPushServiceEnable;

	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* GetRegionInfo;

	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* GetXDGInfoJson;

	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* DisableAgreementUI;

	UPROPERTY(Meta = (BindWidget))
	UComboBoxString* CB_Region_Region;
	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* SetCountryRegion;

	UPROPERTY(Meta = (BindWidget))
	UComboBoxString*  CB_Init_EnvironmentBox;
	UPROPERTY(Meta = (BindWidget))
	UComboBoxString* CB_Init_RegionBox;
	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* DevelopInit;
	
	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* TrackAchievement;
	
	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* EventCompletedTutorial;

	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* EventCreateRole;

	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* ResetPrivacy;

	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* OpenMoment;
	
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_Topic_AppID;
	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* OpenWebTopic;
};
