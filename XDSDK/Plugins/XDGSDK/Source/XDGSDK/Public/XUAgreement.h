#pragma once
#include "XUAgreementBean.h"

class XDGSDK_API XUAgreement {
public:
	static TArray<FXUAgreementBean> GetAgreementList();
	static void ShowDetailAgreement(const FString& Url);
};
