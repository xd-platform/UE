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

void XUAgreement::ShowDetailAgreement(const FString& Url) {
	TUHelper::LaunchURL(*Url, NULL, NULL);
}