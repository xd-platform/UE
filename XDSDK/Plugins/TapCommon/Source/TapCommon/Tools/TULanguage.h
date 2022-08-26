#pragma once

class TAPCOMMON_API TULanguage {
public:
	enum Type {
		// 自动, 会检测UE的国际化，没检测到，会默认返回EN
		AUTO = 0,
		// 简体中文
		ZH_HANS = 1,
		// 英文
		EN = 2,
		// 繁体中文 
		ZH_HANT = 3,
		// 日语
		JA = 4,
		// 韩语
		KO = 5,
		// 泰文
		TH = 6,
		// 印尼文
		ID = 7,
		// 德语
		DE = 8,
		// 法语
		FR = 9,
		// 葡萄牙语
		PT = 10,
		// 西班牙语
		ES = 11,
		// 土耳其语
		TR = 12,
		// 俄罗斯语
		RU = 13,
	};

	static Type GetNativeType();
	static Type GetCurrentType();
	static void SetCurrentType(Type Type);

	DECLARE_EVENT_OneParam(TdsLanguage, LanguageChangedEvent, Type);
	LanguageChangedEvent& OnLanguageChanged() { return LanguageChangedEvent; }


private:
	static TULanguage& Get();
	static TULanguage *SingleInstance;

	TMap<FString, Type> LanguageMap;
	void ParseLanguages();
	Type CurrentType = AUTO;
	LanguageChangedEvent LanguageChangedEvent;
	
};
