#pragma once
#include "TUAccessToken.h"
#include "XUError.h"
#include "XUType.h"

class XUThirdPayHelper
{
public:

	static void StartWebPay(FString PayUrl, TFunction<void(XUType::PayResult Result)> Callback);

private:
	
	static TMap<FString, TFunction<void(XUType::PayResult Result)>> CallbackMap;

	static FString GenerateState(TFunction<void(XUType::PayResult Result)> Callback);
};

