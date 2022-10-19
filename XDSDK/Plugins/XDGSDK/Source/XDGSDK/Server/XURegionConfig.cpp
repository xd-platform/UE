#include "XURegionConfig.h"

#include "XUConfigManager.h"
#include "XUImpl.h"


class XURegionConfigCN: public XURegionConfig
{
public:
	virtual FString BaseHost() {
		return "https://xdsdk-6.xd.cn";
	}

	virtual FString LoginWebHost() {
		return "https://login-xdsdk.xd.cn";
	}
	
	virtual FString TrackHost() {
		return "https://event-tracking-cn.cn-beijing.log.aliyuncs.com";
	}
};

class XURegionConfigIO: public XURegionConfig
{
public:
	virtual FString BaseHost() {
		return "https://xdsdk-intnl-6.xd.com";
	}

	virtual FString LoginWebHost() {
		return "https://login-xdsdk.xd.com";
	}
		
	virtual FString TrackHost() {
		return "https://event-tracking-global.ap-southeast-1.log.aliyuncs.com";
	}
};

TSharedPtr<XURegionConfig> XURegionConfig::Instance = nullptr;

FString XURegionConfig::InitSDKUrl() {
	return BaseHost() / "/api/init/v1/config";
}

FString XURegionConfig::AgreementUrl() {
	return BaseHost() / "/api/init/v1/agreement";
}

FString XURegionConfig::UserProfileUrl() {
	return BaseHost() / "/api/account/v1/info";
}

FString XURegionConfig::CommonLoginUrl() {
	return BaseHost() / "/api/login/v1/union";
}

FString XURegionConfig::LoginSynUrl() {
	return BaseHost() / "/api/login/v1/syn";
}

FString XURegionConfig::BindListUrl() {
	return BaseHost() / "/api/account/v1/bind/list";
}

FString XURegionConfig::BindInterfaceUrl() {
	return BaseHost() / "/api/account/v1/bind";
}

FString XURegionConfig::UnbindInterfaceUrl() {
	return BaseHost() / "/api/account/v1/unbind";
}

FString XURegionConfig::PaybackListUrl() {
	return BaseHost() / "/order/v1/user/repayOrders";
}

FString XURegionConfig::UploadAgreementUrl() {
	return BaseHost() / "/api/account/v1/agreement/confirm";
}

FString XURegionConfig::TrackUrl() {
	return TrackHost() / "logstores/sdk6-prod/track";
}

TSharedPtr<XURegionConfig>& XURegionConfig::Get() {
	if (!Instance.IsValid()) {
		if (XUConfigManager::IsCN()) {
			Instance = MakeShareable(new XURegionConfigCN);
		} else {
			Instance = MakeShareable(new XURegionConfigIO);
		}
	}
	return Instance;
}
