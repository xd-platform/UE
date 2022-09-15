#include "XUPaymentTracker.h"

FString XUPaymentTracker::EventSessionId = "";
FString XUPaymentTracker::PaymentProcessProductId = "";

void XUPaymentTracker::PaymentStart(const FString& ProductID) {
	PaymentProcessProductId = ProductID;
	EventSessionId = FGuid::NewGuid().ToString();
	LogEvent("sdkcharge_request", GetCommonProperties());
}

void XUPaymentTracker::CallPaymentPage() {
	LogEvent("sdkcharge_ask_pay_period", GetCommonProperties());
}

void XUPaymentTracker::PaymentDone() {
	LogEvent("sdkcharge_done", GetCommonProperties());
}

void XUPaymentTracker::LogEvent(const FString& EventName, TSharedPtr<FJsonObject> Properties) {
	Properties->SetStringField("logid", FString::Printf(TEXT("%s%s%lld"), *EventSessionId, *EventName, FDateTime::UtcNow().ToUnixTimestamp()));
	Properties->SetStringField("name", EventName);
	XUTracker::Get()->UploadLog(Properties);
}

TSharedPtr<FJsonObject> XUPaymentTracker::GetCommonProperties() {
	TSharedPtr<FJsonObject> Properties = MakeShareable(new FJsonObject);
	Properties->SetStringField("tag", "sdkcharge");
	Properties->SetStringField("event_session_id", EventSessionId);
	Properties->SetStringField("pay_platform", "PC");
	Properties->SetStringField("pay_channel", "PC");
	Properties->SetStringField("product_id", PaymentProcessProductId);
	return Properties;
}
