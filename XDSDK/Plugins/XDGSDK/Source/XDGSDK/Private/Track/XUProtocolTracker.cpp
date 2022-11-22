#include "XUProtocolTracker.h"

#include "XUTracker.h"
#include "Agreement/XUAgreementManager.h"

FString XUProtocolTracker::EventSessionId = "";

void XUProtocolTracker::AskPrivacy() {
	EventSessionId = FGuid::NewGuid().ToString();
	auto Properties = GetCommonProperties();
	LogEvent("sdkprotocol_privacy_ask", Properties);
}

void XUProtocolTracker::AgreePrivacy(int32 PrivacyType) {
	XUTracker::Get()->UserAgreeProtocol();
	auto Properties = GetCommonProperties();
	Properties->SetStringField("popup_type", FString::Printf(TEXT("%d"), PrivacyType));
	LogEvent("sdkprotocol_privacy_agree", Properties);
}

void XUProtocolTracker::LogEvent(const FString& EventName, TSharedPtr<FJsonObject> Properties) {
	Properties->SetStringField("logid", FString::Printf(TEXT("%s%s%lld"), *EventSessionId, *EventName, FDateTime::UtcNow().ToUnixTimestamp()));
	Properties->SetStringField("name", EventName);
	XUTracker::Get()->UploadLog(Properties);
}

TSharedPtr<FJsonObject> XUProtocolTracker::GetCommonProperties() {
	TSharedPtr<FJsonObject> Properties = MakeShareable(new FJsonObject);
	Properties->SetStringField("event_session_id", EventSessionId);
	Properties->SetStringField("tag", "sdkprotocol");
	auto Ptr = XUAgreementManager::GetCurrentAgreement();
	FString Region = "";
	FString Version = "";
	if (Ptr.IsValid()) {
		Region = Ptr->agreementRegion;
		Version = Ptr->agreementVersion;
	}
	Properties->SetStringField("a_region", Region);
	Properties->SetStringField("a_version", Version);
	return Properties;
}
