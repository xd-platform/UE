#pragma once
#include "TUAccessToken.h"
#include "XUError.h"
#include "XUType.h"

class XUThirdPayHelper
{
public:

	static void StartWebPay(FString PayUrl, TFunction<void(XUType::PayResult Result)> Callback);

	static void CancelAll();

private:
	static TArray<FString> CacheStates;
	
	static TMap<FString, TFunction<void(XUType::PayResult Result)>> CallbackMap;

	static FString GenerateState(TFunction<void(XUType::PayResult Result)> Callback);

	static bool IsContainState(FString State);
};

