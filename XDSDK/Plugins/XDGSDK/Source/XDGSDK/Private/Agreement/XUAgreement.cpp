#include "XUAgreement.h"

#include "TUHelper.h"
#include "XUAgreementManager.h"


TArray<FXUAgreementBean> XUAgreement::GetAgreementList() {
	TArray<FXUAgreementBean> Beans;
	if (XUAgreementManager::GetCurrentAgreement().IsValid()) {
		Beans = XUAgreementManager::GetCurrentAgreement()->agreements;
	}
	return Beans;
}

void XUAgreement::ShowDetailAgreement(const FString Type) {
	for (auto Bean : GetAgreementList()) {
		if (Bean.type == Type) {
			TUHelper::LaunchURL(*Bean.url, NULL, NULL);
		}
	}
}