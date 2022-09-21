#include "XUTracker.h"
#include "TUDeviceInfo.h"
#include "XDGSDK.h"
#include "XDUE.h"
#include "XUConfigManager.h"
#include "XULanguageManager.h"
#include "XULoginTypeModel.h"
#include "XUStorage.h"
#include "XUTrackNet.h"
#include "XUUser.h"
#include "Internationalization/Culture.h"


TSharedPtr<XUTracker> XUTracker::SingtonInstance = nullptr;

TSharedPtr<XUTracker> XUTracker::Get() {
	if (SingtonInstance == nullptr) {
		SingtonInstance = MakeShareable(new XUTracker);
	}
	return SingtonInstance;
}

void XUTracker::UserAgreeProtocol() {
	ProtocolAgreed = true;
	TUDataStorage<FXUStorage>::SaveBool(FXUStorage::ProtocolAgreedKey, true);
	PresetProperties += GetDeviceInfos();
}

void XUTracker::UploadLog(TSharedPtr<FJsonObject> Properties) {
	TSharedPtr<FJsonObject> UploadProperties = MakeShareable(new FJsonObject);
	UploadProperties += PresetProperties;
	UploadProperties += GetCommonProperties();
	UploadProperties += Properties;
	XUTrackNet::UploadTrackInfo(UploadProperties, [](TSharedPtr<TUHttpResponse> Response) {
		
	});
}

XUTracker::XUTracker() {
	ProtocolAgreed = TUDataStorage<FXUStorage>::LoadBool(FXUStorage::ProtocolAgreedKey);
	PresetProperties = GetStaticPresetProperties();
	if (ProtocolAgreed) {
		PresetProperties += GetDeviceInfos();
	}
}

TSharedPtr<FJsonObject> XUTracker::GetDeviceInfos() {
	TSharedPtr<FJsonObject> Properties = MakeShareable(new FJsonObject);
	Properties->SetStringField("device_id", TUDeviceInfo::GetLoginId());
	Properties->SetStringField("os_version", TUDeviceInfo::GetEngineVersion());
	Properties->SetStringField("brand", TUDeviceInfo::GetGPU());
	Properties->SetStringField("model", TUDeviceInfo::GetCPU());
	// provider 运营商
	return Properties;
}

TSharedPtr<FJsonObject> XUTracker::GetCommonProperties() {
	TSharedPtr<FJsonObject> Properties = MakeShareable(new FJsonObject);
	EventIndex += 1;
	Properties->SetStringField("event_index", FString::Printf(TEXT("%lld"), EventIndex));
	int64 Timestamp = (FDateTime::UtcNow() - FDateTime(1970, 1, 1)).GetTotalMilliseconds();
	Properties->SetStringField("time", FString::Printf(TEXT("%lld"), Timestamp));
	if (!ProtocolAgreed) {
		return Properties;
	}
	FString OrientationString = "unknown";
	auto Orientation = FPlatformMisc::GetDeviceOrientation();
	if (Orientation == EDeviceScreenOrientation::Portrait || Orientation == EDeviceScreenOrientation::PortraitUpsideDown) {
		OrientationString = "protrait";
	} else if (Orientation == EDeviceScreenOrientation::LandscapeLeft || Orientation == EDeviceScreenOrientation::LandscapeRight) {
		OrientationString = "landscape";
	}
	Properties->SetStringField("orientation", OrientationString);
	Properties->SetStringField("width", FString::FromInt(TUDeviceInfo::GetScreenWidth()));
	Properties->SetStringField("height", FString::FromInt(TUDeviceInfo::GetScreenHeight()));
	Properties->SetStringField("lang", XULanguageManager::GetLanguageKey());
	auto User = XDUE::GetUserInfo();
	if (User.IsValid()) {
		Properties->SetStringField("account", User->userId);
		Properties->SetStringField("login_type", XULoginTypeModel((XUType::LoginType)User->loginType).TypeName);
	} else {
		// current login type
	}
	return Properties;
}

TSharedPtr<FJsonObject> XUTracker::GetStaticPresetProperties() {
	TSharedPtr<FJsonObject> Properties = MakeShareable(new FJsonObject);
	Properties->SetStringField("source", "client");
	Properties->SetStringField("os", TUDeviceInfo::GetOSVersion());
	Properties->SetStringField("app_version", XUConfigManager::CurrentConfig()->GameVersion);
	Properties->SetStringField("sdk_version", XDUESDK_VERSION);
	Properties->SetStringField("session_uuid", FGuid::NewGuid().ToString());
	FString Channel = XUConfigManager::CurrentConfig()->TapConfig.DBConfig.Channel;
	if (!Channel.IsEmpty()) {
		Properties->SetStringField("channel", Channel);
	}
	Properties->SetStringField("xd_client_id", XUConfigManager::CurrentConfig()->ClientId);
	return Properties;
}
