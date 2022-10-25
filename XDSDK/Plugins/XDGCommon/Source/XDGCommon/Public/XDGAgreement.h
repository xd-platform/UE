#pragma once


#include "CoreMinimal.h"
#include "XDGAgreement.generated.h"

USTRUCT()
struct FXDGAgreement
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString type;

	UPROPERTY()
	FString url;

	FXDGAgreement() = default;
	FXDGAgreement(const FString& Type, const FString& Url): type(Type), url(Url) {};
};