#pragma once
#include "CoreMinimal.h"
#include "XUIpInfoModel.generated.h"

USTRUCT()
struct FXUIpInfoModel
{
	GENERATED_BODY()

	UPROPERTY()
	FString city;

	UPROPERTY()
	FString country;

	UPROPERTY()
	FString country_code;

	UPROPERTY()
	FString latitude;

	UPROPERTY()
	FString longitude;

	UPROPERTY()
	FString src_ip;

	UPROPERTY()
	FString timeZone;

	void SaveToLocal();

	static TSharedPtr<FXUIpInfoModel> GetLocalModel();
};
