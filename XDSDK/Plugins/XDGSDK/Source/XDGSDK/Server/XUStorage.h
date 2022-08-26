#pragma once
#include "TUDataStorage.h"
#include "CoreMinimal.h"
#include "XUStorage.generated.h"

USTRUCT()
struct FXUStorage
{
	GENERATED_BODY()
	static FString InitConfig;
	static FString IpInfo;
	static FString TokenInfo;
	static FString UserInfo;
	// static FString ClientId ;
	static FString PrivacyKey;
	static FString SessionTokenKey;

	static FString XD_CACHE_CONFIG;
	static FString XD_CACHE_AGREEMENT;
	static FString AgreementKRPush;
	
};




