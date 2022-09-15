#pragma once
#include "TUAccessToken.h"
#include "XUError.h"
#include "XUType.h"

class XUThirdPayHelper
{
public:

	static void StartWebPay(FString PayUrl, TFunction<void(XUType::PayResult Result)> Callback);

	static void CancelAllPreAuths();

private:
	static TArray<FString> CacheStates;

	static FString GenerateState();

	static bool JudgeContainStateAndClearAllAuths(FString State);
};
