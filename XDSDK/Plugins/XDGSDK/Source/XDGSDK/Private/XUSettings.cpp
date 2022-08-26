#include "XUSettings.h"

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
		TapUELogin::ChangeLanguage(TUType::ZH);
		break;
	case XUType::ZH_TW:
		TapUELogin::ChangeLanguage(TUType::ZHTW);
		break;
	case XUType::EN:
		TapUELogin::ChangeLanguage(TUType::EN);
		break;
	case XUType::TH:
		TapUELogin::ChangeLanguage(TUType::TH);
		break;
	case XUType::ID:
		TapUELogin::ChangeLanguage(TUType::ID);
		break;
	case XUType::KR:
		TapUELogin::ChangeLanguage(TUType::KO);
		break;
	case XUType::JP:
		TapUELogin::ChangeLanguage(TUType::JA);
		break;
	default:
		TapUELogin::ChangeLanguage(TUType::EN);
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
