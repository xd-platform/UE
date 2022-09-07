#include "XUTracker.h"

TSharedPtr<FJsonObject> XUTracker::GetDeviceInfos() const {
	if (!DeviceInfos.IsValid()) {
		
	}
	return DeviceInfos;
}

TSharedPtr<FJsonObject> XUTracker::GetCommonProperties() const {
	if (!CommonProperties.IsValid()) {
		
	}
	return CommonProperties;
}
