#pragma once
#include "XULanguageModel.h"
#include "XUType.h"

class XULanguageManager
{
public:
	
	static void SetLanguageType(XUType::LangType type);

	static XUType::LangType GetCurrentType();

	static TSharedPtr<FXULanguageModel> GetCurrentModel();

	static FString GetLanguageKey();

	static FString GetCustomerCenterLang();

private:

	static XUType::LangType AnguageType;
	static TSharedPtr<FXULanguageModel> CurrentModel;

	// 用于判断加载模型是否成功过
	static bool LoadModelSuccess; 

	static void UpdateLanguageModel();
};
