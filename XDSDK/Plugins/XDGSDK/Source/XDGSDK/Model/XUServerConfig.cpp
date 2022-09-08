#include "XUServerConfig.h"

#include "TUDebuger.h"
#include "XUNet.h"
#include "XDGSDK.h"

TSharedPtr<FXUServerConfig> FXUServerConfig::CurrentModel = nullptr;

void FXUServerConfig::SaveToLocal()
{
	if (this != CurrentModel.Get())
	{
		CurrentModel = MakeShareable(new FXUServerConfig(*this));
	}
	TUDataStorage<FXUStorage>::SaveStruct(FXUStorage::InitConfig, *this, true);
	// if (CanShowPrivacyAlert())
	// {
	// 	SavePrivacyTxt();
	// }
}

TSharedPtr<FXUServerConfig>& FXUServerConfig::GetLocalModel()
{
	if (CurrentModel == nullptr)
	{
		CurrentModel = TUDataStorage<FXUStorage>::LoadStruct<FXUServerConfig>(FXUStorage::InitConfig);
	}
	return  CurrentModel;
}

bool FXUServerConfig::CanShowPrivacyAlert()
{
	auto md = GetLocalModel();
	if (!md.IsValid()) { // 没有缓存，表示首次启动
		return true;
	}
	auto preStr = TUDataStorage<FXUStorage>::LoadString(FXUStorage::PrivacyKey);
	FString currentStr = FString::Printf(TEXT("%s-%s-%s"), *md->version, *md->configs.serviceAgreementUrl, *md->configs.serviceTermsUrl);
	if (preStr == currentStr)
	{
		return false;
	}
	return true;
}

// void FXUServerConfig::UpdatePrivacyState()
// {
// 	auto md = GetLocalModel();
// 	if (md.IsValid())
// 	{
// 		FString currentStr = FString::Printf(TEXT("%s-%s-%s"), *md->version, *md->configs.serviceAgreementUrl, *md->configs.serviceTermsUrl);
// 		TUDataStorage<FXUStorage>::SaveString(FXUStorage::PrivacyKey, currentStr);
// 	}
// }

// void FXUServerConfig::GetPrivacyTxt(const FString& txtUrl, TFunction<void(FString txt)> callback)
// {
// 	FString *txt = nullptr;
// 	if (CurrentModel.IsValid())
// 	{
// 		txt = CurrentModel->Cache.Find(txtUrl);
// 	}
// 	if (txt == nullptr)
// 	{
// 		XUNet::RequestPrivacyTxt(txtUrl,
// 		[=](FString content)
// 		{
// 			if (CurrentModel.IsValid())
// 			{
// 				CurrentModel->Cache.Add(txtUrl, content);
// 			}
// 			if (callback)
// 			{
// 				callback(content);
// 			}
// 		});
// 	} else
// 	{
// 		callback(*txt);
// 	}
// }

// void FXUServerConfig::SavePrivacyTxt()
// {
// 	GetPrivacyTxt(configs.serviceTermsTxt, nullptr);
// 	GetPrivacyTxt(configs.serviceAgreementTxt, nullptr);
// }
