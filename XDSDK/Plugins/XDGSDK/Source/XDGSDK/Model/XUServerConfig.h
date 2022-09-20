// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XUStorage.h"
#include "XUServerConfig.generated.h"

USTRUCT()
struct FXUServerConfigBindEntries
{
	GENERATED_BODY()
	
	UPROPERTY()
	bool canBind;
	
	UPROPERTY()
	FString entryName;

	UPROPERTY()
	bool canUnbind;
};

USTRUCT()
struct FXUServerConfigTapSdk
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString clientId;
	
	UPROPERTY()
	FString tapDBChannel;
	
	UPROPERTY()
	FString clientToken;
	
	UPROPERTY()
	FString serverUrl;

	UPROPERTY()
	bool enableTapDB;
};

USTRUCT()
struct FXUServerConfigAgreement {
	GENERATED_BODY()

	UPROPERTY()
	FString agreementRegion;
	
	UPROPERTY()
	FString agreementVersion;
	
	UPROPERTY()
	FString agreementUrl;

	UPROPERTY()
	bool isKRPushServiceSwitchEnable = false;
	
};

USTRUCT()
struct FXUServerConfigConfigs
{
	GENERATED_BODY()

	UPROPERTY()
	FString webPayUrlForPC;

	UPROPERTY()
	FXUServerConfigAgreement agreement;

	UPROPERTY()
	FString serviceAgreementUrl;

	UPROPERTY()
	FString serviceAgreementTxt;

	UPROPERTY()
	TArray<FString> gameLogos;

	UPROPERTY()
	bool isProtocolUiEnable;

	UPROPERTY()
	FString googlePlayGamesAndroidClientId;

	UPROPERTY()
	FString serviceTermsUrl;

	UPROPERTY()
	FString serviceTermsTxt;

	UPROPERTY()
	FString reportUrl;

	UPROPERTY()
	TArray<FXUServerConfigBindEntries> bindEntriesConfig;

	UPROPERTY()
	TArray<FString> androidLoginEntries;

	UPROPERTY()
	bool enableFirebaseTrack;

	UPROPERTY()
	TArray<FString> iosLoginEntries;

	UPROPERTY()
	FString californiaPrivacyUrl;

	UPROPERTY()
	FString gameName;

	UPROPERTY()
	FXUServerConfigTapSdk tapSdkConfig;

	UPROPERTY()
	TArray<FString> tapLoginPermissions;

	UPROPERTY()
	TArray<FString> bindEntries;

	UPROPERTY()
	FString appId;

	UPROPERTY()
	FString facebookClientId;

	UPROPERTY()
	TArray<FString> fbLoginPermissions;

	UPROPERTY()
	FString region;

	UPROPERTY()
	FString logoutUrl;

	UPROPERTY()
	bool isKRPushServiceSwitchEnable;
	
};

// 服务 配置文件的Json文件的模型
USTRUCT()
struct FXUServerConfig
{
	GENERATED_BODY()

	UPROPERTY()
	FString version;

	UPROPERTY()
	FString groupId;

	UPROPERTY()
	FXUServerConfigConfigs configs;

	void SaveToLocal();

	static TSharedPtr<FXUServerConfig>& GetLocalModel();

	// 老版判断协议是否弹出过
	static bool CanShowPrivacyAlert();

	// static void UpdatePrivacyState(); //弹过之后若没变化就不再弹出
	
	// static void GetPrivacyTxt(const FString& txtUrl, TFunction<void(FString txt)> callback);

private:
	static TSharedPtr<FXUServerConfig> CurrentModel;
	// void SavePrivacyTxt();

	// TMap<FString, FString> Cache;
};

