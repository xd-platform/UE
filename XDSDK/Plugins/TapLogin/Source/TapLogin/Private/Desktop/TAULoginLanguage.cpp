#include "TAULoginLanguage.h"

#include "TULanguage.h"


TSharedPtr<ILoginLang> TAULoginLanguage::CurrentLang = nullptr;

TSharedPtr<ILoginLang> TAULoginLanguage::GetCurrentLang()
{
	if (CurrentLang.IsValid()) {
		return CurrentLang;
	}
	auto CurrentSysType = TULanguage::GetCurrentType();
	switch (CurrentSysType) {
	case TULanguage::ZH_HANS:
		SetLangType(CN);
		break;
	case TULanguage::EN:
		SetLangType(EN);
		break;
	case TULanguage::ID:
		SetLangType(ID);
		break;
	case TULanguage::JA:
		SetLangType(JA);
		break;
	case TULanguage::KO:
		SetLangType(KO);
		break;
	case TULanguage::TH:
		SetLangType(TH);
		break;
	case TULanguage::ZH_HANT:
		SetLangType(ZHTW);
		break;
	default:
		SetLangType(EN);
		break;
	}
	return CurrentLang;
}

void TAULoginLanguage::SetLangType(Type Type) {
	switch (Type) {
	case CN:
		CurrentLang = MakeShareable(new LoginLangCN);
		break;
	case EN:
		CurrentLang = MakeShareable(new LoginLangIO);
		break;
	case ID:
		CurrentLang = MakeShareable(new LoginLangID);
		break;
	case JA:
		CurrentLang = MakeShareable(new LoginLangJA);
		break;
	case KO:
		CurrentLang = MakeShareable(new LoginLangKO);
		break;
	case TH:
		CurrentLang = MakeShareable(new LoginLangTH);
		break;
	case ZHTW:
		CurrentLang = MakeShareable(new LoginLangZHTW);
		break;
	default:
		CurrentLang = MakeShareable(new LoginLangIO);
		break;
	}
}


