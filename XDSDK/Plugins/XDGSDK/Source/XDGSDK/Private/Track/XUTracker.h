#pragma once

class XUTracker {

public:
	TSharedPtr<FJsonObject> GetDeviceInfos() const;
	TSharedPtr<FJsonObject> GetCommonProperties() const;
private:
	TSharedPtr<FJsonObject> DeviceInfos;
	TSharedPtr<FJsonObject> CommonProperties;
};
