#include "TUMomentPCImpl.h"
#include "TUHelper.h"

void TUMomentPCImpl::Init(const TUMomentType::Config& InitConfig) {
	Config = InitConfig;
}

void TUMomentPCImpl::OpenWebTopic() {
	FString Url;
	if (TUMomentImpl::Get()->Config.RegionType == ERegionType::CN) {
		Url = "https://www.taptap.com/app" / TUMomentImpl::Get()->Config.AppID / "topic?utm_medium=link&utm_source=pc_sdk";
	} else {
		Url = "https://www.taptap.io/app" / TUMomentImpl::Get()->Config.AppID + "?utm_medium=link&utm_source=pc_sdk";
	}
	TUHelper::LaunchURL(*Url, nullptr, nullptr);
}
