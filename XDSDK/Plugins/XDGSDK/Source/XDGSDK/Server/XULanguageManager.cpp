#include "XULanguageManager.h"
#include "TUJsonHelper.h"
#include "JsonObjectConverter.h"
#include "TUDebuger.h"
#include "XDGSDK.h"

static FString LanguageJsonPath = FPaths::ProjectPluginsDir() / TEXT("XDGSDK/Content") / TEXT("XDGAssets/Language.json");

XUType::LangType XULanguageManager::AnguageType = XUType::ZH_CN;
TSharedPtr<FXULanguageModel> XULanguageManager::CurrentModel = nullptr;
bool XULanguageManager::LoadModelSuccess = false;


void XULanguageManager::SetLanguageType(XUType::LangType type)
{
	if (type != AnguageType)
	{
		AnguageType = type;
		UpdateLanguageModel();
	} 
}

XUType::LangType XULanguageManager::GetCurrentType()
{
	return AnguageType;
}

TSharedPtr<FXULanguageModel> XULanguageManager::GetCurrentModel()
{
	if (CurrentModel == nullptr || LoadModelSuccess == false){
		UpdateLanguageModel();
	}
	return CurrentModel;
}



FString XULanguageManager::GetLanguageKey()
{
	if (AnguageType == XUType::ZH_CN){
		return "zh_CN";
	} else if (AnguageType == XUType::ZH_TW){
		return "zh_TW";
	} else if (AnguageType == XUType::EN){
		return "en_US";
	} else if (AnguageType == XUType::TH){
		return "th_TH";
	} else if (AnguageType == XUType::ID){
		return "in_ID";
	} else if (AnguageType == XUType::KR){
		return "ko_KR";
	} else if (AnguageType == XUType::JP){
		return "ja_JP";
	} else if (AnguageType == XUType::DE){
		return "de_DE";
	} else if (AnguageType == XUType::FR){
		return "fr_FR";
	} else if (AnguageType == XUType::PT){
		return "pt_PT";
	} else if (AnguageType == XUType::ES){
		return "es_ES";
	} else if (AnguageType == XUType::TR){
		return "tr_TR";
	} else if (AnguageType == XUType::RU){
		return "ru_RU";
	} else if (AnguageType == XUType::VI){
		return "vi_VN";
	}
	return "en_US";
}

FString XULanguageManager::GetCustomerCenterLang()
{
	if (AnguageType == XUType::ZH_CN){
		return "cn";
	} else if (AnguageType == XUType::ZH_TW){
		return "tw";
	} else if (AnguageType == XUType::EN){
		return "us";
	} else if (AnguageType == XUType::TH){
		return "th";
	} else if (AnguageType == XUType::ID){
		return "id";
	} else if (AnguageType == XUType::KR){
		return "kr";
	} else if (AnguageType == XUType::JP){
		return "jp";
	} else if (AnguageType == XUType::DE){
		return "de";
	} else if (AnguageType == XUType::FR){
		return "fr";
	} else if (AnguageType == XUType::PT){
		return "pt";
	} else if (AnguageType == XUType::ES){
		return "es";
	} else if (AnguageType == XUType::TR){
		return "tr";
	} else if (AnguageType == XUType::RU){
		return "ru";
	} else if (AnguageType == XUType::VI){
		return "vi";
	}
	return "us";
}

void XULanguageManager::UpdateLanguageModel()
{
	FString JsonStr;
	if (FFileHelper::LoadFileToString(JsonStr, *LanguageJsonPath)) {
		TSharedPtr<FJsonObject> JsonObject = TUJsonHelper::GetJsonObject(JsonStr);
		const TSharedPtr<FJsonObject>* languageObject = nullptr;
		if (JsonObject.IsValid() && JsonObject->TryGetObjectField(GetLanguageKey(), languageObject)) {
			CurrentModel = MakeShareable(new FXULanguageModel);
			if (FJsonObjectConverter::JsonObjectToUStruct(languageObject->ToSharedRef(), CurrentModel.Get())) {
				LoadModelSuccess = true;
			}
			else {
				TUDebuger::ErrorLog(FString::Printf(TEXT("%s language json content error"), *GetLanguageKey()));
			}
		}
		else {
			TUDebuger::ErrorLog("language json content error");
		}

	}
	else {
		TUDebuger::ErrorLog("not found language json");
	}
	// 保底，如果CurrentModel加载没有成功，那么给个空对象，别让调用崩溃了。
	if (!CurrentModel.IsValid()) {
		CurrentModel = MakeShareable(new FXULanguageModel);
	}
}











