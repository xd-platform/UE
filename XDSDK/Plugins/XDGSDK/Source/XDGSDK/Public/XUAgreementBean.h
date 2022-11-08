#pragma once
#include "CoreMinimal.h"
#include "XUAgreementBean.generated.h"

USTRUCT()
struct FXUAgreementBean
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString type;

	UPROPERTY()
	FString url;

    FXUAgreementBean() = default;
	FXUAgreementBean(const FString& Type, const FString& Url);
};