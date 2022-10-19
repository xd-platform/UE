#include "XUConfigManager.h"

#include "TapUEBootstrap.h"
#include "TUDataStorage.h"
#include "TUHelper.h"
#include "XUImpl.h"
#include "XULanguageManager.h"
#include "XULocalConfig.h"
#include "XUNet.h"
#include "XUStorage.h"
#include "XUServerConfig.h"
#include "Agreement/XUAgreementManager.h"

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

void XUConfigManager::UpdateConfigWithCache() {
	UpdateConfig(FXUServerConfig::GetLocalModel());
}

void XUConfigManager::RequestServerConfig() {
	XUNet::RequestConfig([=](TSharedPtr<FXUServerConfig> Model, FXUError Error) {
		if (Model.IsValid()) {
			Model->SaveToLocal();
			// 海外此时单独用接口值刷新一下
			UpdateConfig(Model);

		} else {

		}
	});
}

void XUConfigManager::InitTapSDK() {
	if (!TapTapEnable()) {
		return;
	}
	TapUEBootstrap::Init(CurrentConfig()->TapConfig);
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

bool XUConfigManager::IsGameInited() {
	return XUImpl::Get()->InitState == XUImpl::Inited;
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
	auto CurrentAgreement = XUAgreementManager::GetCurrentAgreement();
	if (!CurrentAgreement.IsValid()) {
		return false;
	}
	bool IsKR = CurrentAgreement->agreementRegion == "kr";
	bool CanPush = CurrentAgreement->isKRPushServiceSwitchEnable;
	if (IsKR && CanPush) {
		return true;
	}
	return false;
}

bool XUConfigManager::IsGameInNA() {
	auto CurrentAgreement = XUAgreementManager::GetCurrentAgreement();
	if (!CurrentAgreement.IsValid()) {
		return false;
	}
	return CurrentAgreement->agreementRegion == "us";
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
	
	if (!ServerConfig->configs.region.IsEmpty()) {
		config->Region = ServerConfig->configs.region;
	}
	if (!SharedInstance().TargetRegion.IsEmpty()) {
		config->Region = SharedInstance().TargetRegion;
	}
	
	SetConfig(config);
	
}
