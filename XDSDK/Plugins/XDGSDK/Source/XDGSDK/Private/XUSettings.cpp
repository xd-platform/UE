#include "XUSettings.h"

#include "TapUECommon.h"
#include "TapUELogin.h"
#include "TUSettings.h"
#include "XUConfigManager.h"
#include "XULanguageManager.h"
#include "XULocalConfig.h"

void XUSettings::SetDebugMode(bool Debug) {
}

void XUSettings::SetLanguage(XUType::LangType Locale) {
	XULanguageManager::SetLanguageType(Locale);
	switch (Locale) {
	case XUType::ZH_CN:
		TapUECommon::SetLanguage(ELanguageType::ZH);
		break;
	case XUType::ZH_TW:
		TapUECommon::SetLanguage(ELanguageType::ZHTW);
		break;
	case XUType::EN:
		TapUECommon::SetLanguage(ELanguageType::EN);
		break;
	case XUType::TH:
		TapUECommon::SetLanguage(ELanguageType::TH);
		break;
	case XUType::ID:
		TapUECommon::SetLanguage(ELanguageType::ID);
		break;
	case XUType::KR:
		TapUECommon::SetLanguage(ELanguageType::KO);
		break;
	case XUType::JP:
		TapUECommon::SetLanguage(ELanguageType::JA);
		break;
	default:
		TapUECommon::SetLanguage(ELanguageType::EN);
		break;
	}
}

void XUSettings::SetTargetCountryOrRegion(const FString& TargetCountryOrRegion) {
	XUConfigManager::SharedInstance().TargetRegion = TargetCountryOrRegion;
}

void XUSettings::SetAgreementUIEnable(bool Enable) {
	XUConfigManager::SharedInstance().AgreementUIEnable = Enable;
}

void XUSettings::UpdateConfigFileName(const FString& FileName) {
	XUConfigManager::SharedInstance().ConfigFileName = FileName;
}

void XUSettings::SetUILevel(int32 Level) {
	TUSettings::SetUILevel(Level);
}

void XUSettings::SetGameInstance(TWeakObjectPtr<UGameInstance> GameInstance) {
	TUSettings::SetGameInstance(GameInstance);
}
