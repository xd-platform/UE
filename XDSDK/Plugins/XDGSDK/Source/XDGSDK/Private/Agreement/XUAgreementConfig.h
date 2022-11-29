#pragma once
#include "CoreMinimal.h"
#include "XUAgreementBean.h"
#include "XUAgreementConfig.generated.h"

USTRUCT()
struct FXUAgreementConfig
{
	GENERATED_BODY()
	
	UPROPERTY()
	bool isKRPushServiceSwitchEnable;

	UPROPERTY()
	FString dataCollectionAgreementUrl;
	
	UPROPERTY()
	FString agreementUrl;

	UPROPERTY()
	FString agreementVersion;

	UPROPERTY()
	FString agreementRegion;

	UPROPERTY()
	TArray<FXUAgreementBean> agreements;

	static  TSharedPtr<FXUAgreementConfig> GenerateDefaultConfig(bool IsCN);
};
