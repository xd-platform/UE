#pragma once
#include "XUTracker.h"

class XUPaymentTracker{
public:
	// 开始支付
	static void PaymentStart(const FString& ProductID);
	// 唤起支付页面
	static void CallPaymentPage();
	// 完成支付
	static void PaymentDone();
	
private:
	static void LogEvent(const FString& EventName, TSharedPtr<FJsonObject> Properties);
	static TSharedPtr<FJsonObject> GetCommonProperties();

	static FString EventSessionId;
	static FString PaymentProcessProductId;
};
