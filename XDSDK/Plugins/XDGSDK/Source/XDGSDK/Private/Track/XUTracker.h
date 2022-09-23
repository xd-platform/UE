#pragma once

class XUTracker {

public:
	static TSharedPtr<XUTracker> Get();
	void UserAgreeProtocol();

	void UploadLog(TSharedPtr<FJsonObject> Properties);
private:
	XUTracker();
	static TSharedPtr<XUTracker> SingtonInstance;
	
	TSharedPtr<FJsonObject> PresetProperties;
	int64 EventIndex;

	bool ProtocolAgreed;
	TSharedPtr<FJsonObject> GetDeviceInfos();
	TSharedPtr<FJsonObject> GetCommonProperties();
	TSharedPtr<FJsonObject> GetStaticPresetProperties();
};
