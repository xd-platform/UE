#include "XULoginTracker.h"

#include "XUTracker.h"

FString XULoginTracker::EventSessionId = "";

FString XULoginTracker::GetCurrentSessionId() {
	return EventSessionId;
}

void XULoginTracker::ClearCurrentSessionId() {
	EventSessionId = "";
}

void XULoginTracker::LoginStart() {
	EventSessionId = FGuid::NewGuid().ToString();
	auto Properties = GetCommonProperties();
	LogEvent("sdklogin_start", Properties);
}

void XULoginTracker::LoginRiskSuccess(FXUError Error) {
	if (Error.code != 40101 &&
		Error.code != 40107 &&
		Error.code != 40108 &&
		Error.code != 40109 &&
		Error.code != 42900) {
		return;
	}
	auto Properties = GetCommonProperties();
	Properties->SetStringField("event_reason", Error.msg);
	LogEvent("sdklogin_risk_success", Properties);
}

void XULoginTracker::Login2Authorize() {
	auto Properties = GetCommonProperties();
	LogEvent("sdklogin_to_authorize", Properties);
}

void XULoginTracker::Login2AuthorizeSuccess() {
	auto Properties = GetCommonProperties();
	LogEvent("sdklogin_authorize_success", Properties);
}

void XULoginTracker::Login2AuthorizeFailed(const FString& Reason) {
	auto Properties = GetCommonProperties();
	Properties->SetStringField("event_reason", Reason);
	LogEvent("sdklogin_authorize_fail", Properties);
}

void XULoginTracker::LoginPreLoginSuccess() {
	auto Properties = GetCommonProperties();
	LogEvent("sdklogin_pre_login_success", Properties);
}

void XULoginTracker::LoginPreLoginFailed(const FString& Reason) {
	auto Properties = GetCommonProperties();
	Properties->SetStringField("event_reason", Reason);
	LogEvent("sdklogin_pre_login_fail", Properties);
}

void XULoginTracker::LoginSuccess() {
	auto Properties = GetCommonProperties();
	LogEvent("sdklogin_success", Properties);
}

void XULoginTracker::LoginFailed(const FString& Reason) {
	auto Properties = GetCommonProperties();
	Properties->SetStringField("event_reason", Reason);
	LogEvent("sdklogin_fail", Properties);
}

void XULoginTracker::Logout(const FString& Reason) {
	auto Properties = GetCommonProperties();
	Properties->SetStringField("event_reason", Reason);
	LogEvent("sdklogin_logout", Properties);
}

void XULoginTracker::LogEvent(const FString& EventName, TSharedPtr<FJsonObject> Properties) {
	Properties->SetStringField("logid", FString::Printf(TEXT("%s%s%lld"), *EventSessionId, *EventName, FDateTime::UtcNow().ToUnixTimestamp()));
	Properties->SetStringField("name", EventName);
	XUTracker::Get()->UploadLog(Properties);
}

TSharedPtr<FJsonObject> XULoginTracker::GetCommonProperties() {
	TSharedPtr<FJsonObject> Properties = MakeShareable(new FJsonObject);
	Properties->SetStringField("tag", "sdklogin");
	Properties->SetStringField("event_session_id", EventSessionId);
	return Properties;
}
