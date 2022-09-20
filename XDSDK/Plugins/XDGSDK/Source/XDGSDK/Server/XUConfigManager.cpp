#include "XUConfigManager.h"

#include "TapUEBootstrap.h"
#include "TUDataStorage.h"
#include "TUDeviceInfo.h"
#include "TUHelper.h"
#include "XULanguageManager.h"
#include "XULocalConfig.h"
#include "XUNet.h"
#include "XUSettings.h"
#include "XUStorage.h"
#include "XUServerConfig.h"

XUConfigManager * XUConfigManager::Instance = nullptr;

XUConfigManager& XUConfigManager::SharedInstance() {
	if (Instance == nullptr) {
		Instance = new XUConfigManager();
		Instance->ConfigFileName = "XDConfig.json";
		Instance->Config = MakeShareable(new XUType::Config);
		Instance->AgreementUIEnable = true;
	}
	return *Instance;
}

TSharedPtr<XUType::Config> XUConfigManager::CurrentConfig() {
	return SharedInstance().Config;
}

void XUConfigManager::SetConfig(TSharedPtr<XUType::Config> Config) {
	SharedInstance().Config = Config;
	// UpdateHttpConfig();
}

bool XUConfigManager::IsCN() {
	return CurrentConfig()->RegionType == XUType::CN;
}

void XUConfigManager::ReadLocalConfig(XUConfigHandler Handler) {
	auto Ptr = FXULocalConfig::GetLocalModel();
	if (!Ptr.IsValid()) {
		if (Handler) {
			Handler(nullptr, "Config file empty, Please check your XDConfig.json");
		}
		return;
	}
	auto InitPtr = Ptr->GenerateXUConfig();
	SetConfig(InitPtr);
	if (Handler) {
		Handler(InitPtr, "");
	}
}

void XUConfigManager::LoadRemoteOrCachedServiceTerms(TSharedPtr<XUType::Config> Config, XUConfigHandler Handler) {
	// 统一先用cache刷新一下
	UpdateConfig(FXUServerConfig::GetLocalModel());
	if (Config->RegionType == XUType::CN) {
		if (Handler) {
			Handler(Config, "");
		}
	} else {
		RequestServerConfig(true, [=](bool Success) {
			if (Handler) {
				Handler(Config, "");
			}
		});
	}
}

void XUConfigManager::RequestServerConfig(bool FirstRequest) {
	if (SharedInstance().ConfigRequestSuccess) {
		return;
	}
	RequestServerConfig(FirstRequest, [](bool Success) {
		
	});
}

void XUConfigManager::RequestServerConfig(bool FirstRequest, TFunction<void(bool Success)> Handler) {
	XUNet::RequestConfig(FirstRequest, [=](TSharedPtr<FXUServerConfig> Model, FXUError Error) {
		if (Model.IsValid()) {
			if (IsCN()) {
				auto localAgreementDic = TUDataStorage<FXUStorage>::LoadJsonObject(GetRegionAgreementCacheName());
				XUType::AgreementConfig agreementConfig;
				if (localAgreementDic.IsValid() && localAgreementDic->Values.Num() > 0) {
					agreementConfig = GenerateAgreementConfig(localAgreementDic);
				} else {
					agreementConfig = CurrentConfig()->Agreement;
				}
				if (!agreementConfig.Version.IsEmpty()) {
					if (agreementConfig.Version == "latest") {
						if (!Model->configs.agreement.agreementVersion.IsEmpty()) {
							agreementConfig.Version = Model->configs.agreement.agreementVersion;
							SaveAgreementConfig(agreementConfig, false);
						}
					}
				}
			}
			Model->SaveToLocal();
			UpdateConfig(Model);
			SharedInstance().ConfigRequestSuccess = true;
			if (Handler) {
				Handler(true);
			}
		} else {
			if (Handler) {
				Handler(false);
			}
		}
	});
}

void XUConfigManager::InitTapSDK() {
	if (!TapTapEnable()) {
		return;
	}

	TapUEBootstrap::Init(CurrentConfig()->TapConfig);
}

bool XUConfigManager::NeedShowAgreement() {
	if (SharedInstance().AgreementUIEnable == false) {
		return false;
	}
	XUType::AgreementConfig& currentAgreement = CurrentConfig()->Agreement;
	if (currentAgreement.Url.IsEmpty()) {
		return false;
	}
	auto localAgreementDic = TUDataStorage<FXUStorage>::LoadJsonObject(GetRegionAgreementCacheName());
	if (localAgreementDic.IsValid() && localAgreementDic->Values.Num() > 0) {
		XUType::AgreementConfig localCacheAgreementConfig = GenerateAgreementConfig(localAgreementDic);
		if (!localCacheAgreementConfig.Version.IsEmpty()) {
			FString localCacheAgreementStr = localCacheAgreementConfig.Version + localCacheAgreementConfig.Region;
			FString currentAgreementStr = currentAgreement.Version + currentAgreement.Region;
			if (localCacheAgreementStr != currentAgreementStr) {
				// 接口版本和缓存版本不一致时需要签署
				return true;
			}
		}
	} else {
		// 判断老版本数据
		if (FXUServerConfig::CanShowPrivacyAlert()) {
			return true;
		} else {
			SaveAgreementConfig(currentAgreement, false);
		}
	}
	return false;
}

FString XUConfigManager::GetAgreementUrl() {
	FString BaseUrl = CurrentConfig()->Agreement.Url;
	auto LocalAgreementDic = TUDataStorage<FXUStorage>::LoadJsonObject(GetRegionAgreementCacheName());

	TSharedPtr<FJsonObject> Paras = MakeShareable(new FJsonObject);
	Paras->SetStringField("client_id", CurrentConfig()->ClientId);
	Paras->SetStringField("language", XULanguageManager::GetLanguageKey());
	if (!SharedInstance().TargetRegion.IsEmpty()) {
		Paras->SetStringField("region", SharedInstance().TargetRegion);
	}
	if (LocalAgreementDic.IsValid() && LocalAgreementDic->Values.Num() > 0) {
		Paras->SetStringField("firstCheck", "false");
	} else {
		Paras->SetStringField("firstCheck", "true");
	}
	FString Url = BaseUrl + "?" + TUHelper::CombinParameters(Paras);
	return Url;
}

void XUConfigManager::UploadUserAgreement() {
	if (CurrentConfig()->Agreement.Version.IsEmpty()) {
		TUDebuger::ErrorLog("Agreement Version Is Empty");
		return;
	}
	bool Upload = false;
	auto LocalAgreementDic = TUDataStorage<FXUStorage>::LoadJsonObject(GetRegionAgreementCacheName());
	if (LocalAgreementDic.IsValid() && LocalAgreementDic->Values.Num() > 0) {
		auto localCacheAgreementConfig = GenerateAgreementConfig(LocalAgreementDic);
		if (!localCacheAgreementConfig.Version.IsEmpty()) {
			FString LocalAgreementCacheStr = localCacheAgreementConfig.Version + localCacheAgreementConfig.Region;
			auto currentAgreement = CurrentConfig()->Agreement;
			FString currentAgreementStr = currentAgreement.Version + currentAgreement.Region;
			if (LocalAgreementCacheStr == currentAgreementStr) {
				bool oldUpload = LocalAgreementDic->GetBoolField("upload");
				if (oldUpload) {
					// 接口版本和缓存版本一致且已上报过时不再上报
					Upload = true;
				}
			}
		}
	}

	if (!Upload) {
		TSharedPtr<FJsonObject> postData = MakeShareable(new FJsonObject);
		postData->SetStringField("clientId", CurrentConfig()->ClientId);
		postData->SetStringField("deviceCode", TUDeviceInfo::GetLoginId());
		postData->SetStringField("agreementVersion", CurrentConfig()->Agreement.Version);
		postData->SetStringField("agreementRegion", CurrentConfig()->Agreement.Region);
		XUNet::UploadAgreement(postData, [](TSharedPtr<FXUUploadAgreementResultModel> Model, FXUError Error) {
			if (Model.IsValid()) {
				SaveAgreementConfig(CurrentConfig()->Agreement, Model->isSuccess);
			} else {
				SaveAgreementConfig(CurrentConfig()->Agreement, false);
			}
		});
	}
}


void XUConfigManager::GetRegionInfo(TFunction<void(TSharedPtr<FXUIpInfoModel> ModelPtr)> ResultBlock) {
	XUNet::RequestIpInfo([=](TSharedPtr<FXUIpInfoModel> model, FXUError error) {
		if (model == nullptr) {
			TSharedPtr<FXUIpInfoModel> infoModel = FXUIpInfoModel::GetLocalModel();
			if (ResultBlock) { ResultBlock(infoModel); }
		}
		else {
			model->SaveToLocal();
			if (ResultBlock) { ResultBlock(model); }
		}
	});
}

// void XUConfigManager::UpdateHttpConfig() {
// }

void XUConfigManager::SetGameInited() {
	SharedInstance().IsInited = true;
}

bool XUConfigManager::IsGameInited() {
	return SharedInstance().IsInited;
}

bool XUConfigManager::GoogleEnable() {
	if (!CurrentConfig()->GoogleInfo.ClientID.IsEmpty()) {
		return true;
	}
	return false;
}

// bool XUConfigManager::FacebookEnable() {
// 	if (!CurrentConfig()->FacebookInfo.AppID.IsEmpty()) {
// 		return true;
// 	}
// 	return false;
// }

bool XUConfigManager::TapTapEnable() {
	if (!CurrentConfig()->TapConfig.ClientID.IsEmpty()) {
		return true;
	}
	return false;
}

bool XUConfigManager::TapDBEnable() {
	if (TapTapEnable() && CurrentConfig()->TapConfig.DBConfig.Enable) {
		return true;
	}
	return false;
}

// bool XUConfigManager::AdjustEnable() {
// 	if (!CurrentConfig()->AdjustInfo.AppToken.IsEmpty()) {
// 		return true;
// 	}
// 	return false;
// }

// bool XUConfigManager::AppsflyersEnable() {
// 	if (!CurrentConfig()->AppsflyerInfo.AppID.IsEmpty()) {
// 		return true;
// 	}
// 	return false;
// }

// bool XUConfigManager::LineEnable() {
// 	if (!CurrentConfig()->LineInfo.ChannelID.IsEmpty()) {
// 		return true;
// 	}
// 	return false;
// }

// bool XUConfigManager::TwitterEnable() {
// 	if (!CurrentConfig()->TwitterInfo.ConsumerKey.IsEmpty() && !CurrentConfig()->TwitterInfo.ConsumerSecret.IsEmpty()) {
// 		return true;
// 	}
// 	return false;
// }

bool XUConfigManager::NeedReportService() {
	if (!CurrentConfig()->ReportUrl.IsEmpty()) {
		return true;
	}
	return false;
}

bool XUConfigManager::IsGameInKoreaAndPushServiceEnable() {
	bool IsKR = CurrentConfig()->Agreement.Region == "kr";
	bool CanPush = CurrentConfig()->Agreement.IsKRPushServiceSwitchEnable;
	if (IsKR && CanPush) {
		return true;
	}
	return false;
}

bool XUConfigManager::IsGameInNA() {
	return CurrentConfig()->Agreement.Region == "us";
}

void XUConfigManager::RecordKRPushSetting(bool PushOn) {
	TUDataStorage<FXUStorage>::SaveBool(FXUStorage::AgreementKRPush, PushOn);
}

bool XUConfigManager::GetKRPushSetting() {
	if (IsGameInKoreaAndPushServiceEnable() && TUDataStorage<FXUStorage>::LoadBool(FXUStorage::AgreementKRPush)) {
		return true;
	}
	return false;
}

FString XUConfigManager::GetRegionAgreementCacheName() {
	return FXUStorage::XD_CACHE_AGREEMENT + CurrentConfig()->Agreement.Region;
}

XUType::AgreementConfig XUConfigManager::GenerateAgreementConfig(const TSharedPtr<FJsonObject>& JsonObject) {
	
	XUType::AgreementConfig AgreementConfig;
	if (!JsonObject.IsValid()) {
		return AgreementConfig;
	}
	AgreementConfig.Url = JsonObject->GetStringField("agreementUrl");
	AgreementConfig.Version = JsonObject->GetStringField("agreementVersion");
	AgreementConfig.Region = JsonObject->GetStringField("agreementRegion");
	AgreementConfig.IsKRPushServiceSwitchEnable = JsonObject->GetBoolField("isKRPushServiceSwitchEnable");
	return AgreementConfig;
}

void XUConfigManager::SaveAgreementConfig(XUType::AgreementConfig& AgreementConfig, bool Upload) {
	TSharedPtr<FJsonObject> PostData = MakeShareable(new FJsonObject);
	PostData->SetStringField("clientId", CurrentConfig()->ClientId);
	PostData->SetStringField("deviceCode", TUDeviceInfo::GetLoginId());
	PostData->SetStringField("agreementVersion", AgreementConfig.Version);
	PostData->SetStringField("agreementRegion", AgreementConfig.Region);
	PostData->SetStringField("agreementUrl", AgreementConfig.Url);
	PostData->SetBoolField("isKRPushServiceSwitchEnable", AgreementConfig.IsKRPushServiceSwitchEnable);
	PostData->SetBoolField("upload", Upload);
	TUDataStorage<FXUStorage>::SaveJsonObject(GetRegionAgreementCacheName(), PostData);
}

void XUConfigManager::UpdateConfig(TSharedPtr<FXUServerConfig> ServerConfig) {
	if (!ServerConfig.IsValid()) {
		return;
	}
	auto config = CurrentConfig();
	config->BindEntries.Empty();
	for (auto BindEntry : ServerConfig->configs.bindEntriesConfig) {
		XUType::BindEntriesConfig BindEntriesConfig;
		BindEntriesConfig.CanBind = BindEntry.canBind;
		BindEntriesConfig.CanUnbind = BindEntry.canUnbind;
		BindEntriesConfig.EntryName = BindEntry.entryName;
		config->BindEntries.Add(BindEntriesConfig);
	}

	if (!ServerConfig->configs.reportUrl.IsEmpty()) {
		config->ReportUrl = ServerConfig->configs.reportUrl;
	}

	if (!ServerConfig->configs.logoutUrl.IsEmpty()) {
		config->LogoutUrl = ServerConfig->configs.logoutUrl;
	}

	if (!ServerConfig->configs.webPayUrlForPC.IsEmpty()) {
		config->WebPayUrl = ServerConfig->configs.webPayUrlForPC;
	}

	if (!ServerConfig->configs.appId.IsEmpty()) {
		config->AppID = ServerConfig->configs.appId;
	}

	if (!ServerConfig->configs.agreement.agreementVersion.IsEmpty()) {
		config->Agreement.Version = ServerConfig->configs.agreement.agreementVersion;
		config->Agreement.Url = ServerConfig->configs.agreement.agreementUrl;
		config->Agreement.Region = ServerConfig->configs.agreement.agreementRegion;
		config->Agreement.IsKRPushServiceSwitchEnable = ServerConfig->configs.agreement.isKRPushServiceSwitchEnable;
	}

	if (config->Region.IsEmpty()) {
		config->Region = ServerConfig->configs.region;
	}
	if (!SharedInstance().TargetRegion.IsEmpty()) {
		config->Region = SharedInstance().TargetRegion;
	}
	
	SetConfig(config);
	
}
