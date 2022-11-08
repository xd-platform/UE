#include "XUAgreementConfig.h"

TSharedPtr<FXUAgreementConfig> FXUAgreementConfig::GenerateDefaultConfig(bool IsCN) {
	if (IsCN) {
		TSharedPtr<FXUAgreementConfig> ConfigPtr = MakeShareable(new FXUAgreementConfig);
		ConfigPtr->agreementUrl = "https://protocol.xd.cn/sdk/merger.html";
		ConfigPtr->agreementVersion = "latest";
		ConfigPtr->agreementRegion = "DF";
		ConfigPtr->isKRPushServiceSwitchEnable = false;
		ConfigPtr->agreements.Add(FXUAgreementBean("terms-of-service", "https://protocol.xd.cn/sdk-agreement-1.0.html"));
		ConfigPtr->agreements.Add(FXUAgreementBean("privacy-policy", "https://protocol.xd.cn/sdk-privacy-1.0.html"));
		ConfigPtr->agreements.Add(FXUAgreementBean("children-protection-rules", "https://protocol.xd.cn/sdk-child-protection-1.0.html"));
		ConfigPtr->agreements.Add(FXUAgreementBean("sharing-checklist", "https://protocol.xd.cn/sdk-share-1.0.html"));
		return ConfigPtr;
	} else {
		return nullptr;
	}
}
