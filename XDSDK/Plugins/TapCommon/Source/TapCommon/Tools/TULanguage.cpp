#include "TULanguage.h"
#include "Internationalization/Culture.h"

TULanguage* TULanguage::SingleInstance = nullptr;

TULanguage& TULanguage::Get() {
	if (SingleInstance == nullptr) {
		SingleInstance = new TULanguage();
		SingleInstance->ParseLanguages();
		FInternationalization::Get().OnCultureChanged().AddLambda([]() {
			SingleInstance->LanguageChangedEvent.Broadcast(SingleInstance->GetCurrentType());
		});
	}
	return *SingleInstance;
}

void TULanguage::ParseLanguages() {
	TArray<FString> Names = FTextLocalizationManager::Get().GetLocalizedCultureNames(ELocalizationLoadFlags::Game);
	for (auto Name : Names) {
		if (Name.StartsWith("zh")) {
			if (Name == "zh-TW") {
				LanguageMap.Add(Name, ZH_HANS);
			} else {
				LanguageMap.Add(Name, ZH_HANT);
			}
		} else if (Name.StartsWith("en")) {
			LanguageMap.Add(Name, EN);
		} else if (Name.StartsWith("de")) {
			LanguageMap.Add(Name, DE);
		} else if (Name.StartsWith("es")) {
			LanguageMap.Add(Name, ES);
		} else if (Name.StartsWith("fr")) {
			LanguageMap.Add(Name, FR);
		} else if (Name.StartsWith("id")) {
			LanguageMap.Add(Name, ID);
		} else if (Name.StartsWith("ja")) {
			LanguageMap.Add(Name, JA);
		} else if (Name.StartsWith("ko")) {
			LanguageMap.Add(Name, KO);
		} else if (Name.StartsWith("pt")) {
			LanguageMap.Add(Name, PT);
		} else if (Name.StartsWith("ru")) {
			LanguageMap.Add(Name, RU);
		} else if (Name.StartsWith("th")) {
			LanguageMap.Add(Name, TH);
		} else if (Name.StartsWith("tr")) {
			LanguageMap.Add(Name, TR);
		} 
	}
}

TULanguage::Type TULanguage::GetNativeType() {
	FString Name = FTextLocalizationManager::Get().GetNativeCultureName(ELocalizedTextSourceCategory::Game);
	const Type * FindKey = Get().LanguageMap.Find(Name);
	if (FindKey == nullptr) {
		return EN;
	}
	return *FindKey;
}

TULanguage::Type TULanguage::GetCurrentType() {
	if (Get().CurrentType != AUTO) {
		return Get().CurrentType;
	}
	FString Name = FInternationalization::Get().GetCurrentCulture().Get().GetName();
	const Type * FindKey = Get().LanguageMap.Find(Name);
	if (FindKey == nullptr) {
		return EN;
	}
	return *FindKey;
}

void TULanguage::SetCurrentType(Type Type) {
	Get().CurrentType = Type;
}




