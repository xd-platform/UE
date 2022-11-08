#pragma once
#include "XUAgreementConfig.h"

class XUAgreementManager {
public:
	static XUAgreementManager& SharedInstance();
	XUAgreementManager();

	static TSharedPtr<FXUAgreementConfig> GetCurrentAgreement();

	static void RequestServerAgreementsExceptCN(TFunction<void(bool Success)> Handler);

	static void RequestServerAgreements(bool InitTime, TFunction<void(bool Success)> Handler);

	static void CheckAgreementWithHandler(TFunction<void()> Handler);

	static FString GetAgreementUrl();

	static void ResetAgreement();
	
private:
	bool RequestSuccess = false;
	bool HasSignedAgreement = false;
	TSharedPtr<FXUAgreementConfig> CurrentAgreement;

	static TSharedPtr<XUAgreementManager> Instance;

	void UploadUserAgreement();
	void ShowAgreementWhenLogout();
	static FString GetRegionAgreementCacheName();

	static void SaveAgreementConfig(TSharedPtr<FXUAgreementConfig> AgreementConfig, bool Upload);

	static bool NeedShowAgreement();

};
