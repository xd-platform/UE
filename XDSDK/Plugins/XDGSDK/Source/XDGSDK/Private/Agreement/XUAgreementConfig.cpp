#include "XUAgreementConfig.h"

TSharedPtr<FXUAgreementConfig> FXUAgreementConfig::GenerateDefaultConfig(bool IsCN) {
	if (IsCN) {
		TSharedPtr<FXUAgreementConfig> ConfigPtr = MakeShareable(new FXUAgreementConfig);
		ConfigPtr->agreementUrl = "https://protocol.xd.cn/sdk/merger.html";
		ConfigPtr->agreementVersion = "latest";
		ConfigPtr->agreementRegion = "DF";
		ConfigPtr->isKRPushServiceSwitchEnable = false;
		return ConfigPtr;
	} else {
		return nullptr;
	}
}
