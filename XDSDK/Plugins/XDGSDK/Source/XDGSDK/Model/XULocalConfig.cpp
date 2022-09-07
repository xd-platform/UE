#include "XULocalConfig.h"

#include "TUDebuger.h"
#include "XUConfigManager.h"

static FString InitLocalConfigPath = FPaths::ProjectPluginsDir() / TEXT("XDGSDK/Content/XDGAssets");

TSharedPtr<FXULocalConfig> FXULocalConfig::GetLocalModel() {
	FString FilePath = InitLocalConfigPath / XUConfigManager::SharedInstance().ConfigFileName;
	FString JsonStr;
	if (FFileHelper::LoadFileToString(JsonStr, *FilePath)) {
		TSharedPtr<FXULocalConfig> Ptr = TUJsonHelper::GetUStruct<FXULocalConfig>(JsonStr);
		if (Ptr.IsValid()) {
			return Ptr;
		}
		else {
			TUDebuger::ErrorLog("Config Json Parse Error");
			return nullptr;
		}
	}
	else {
		TUDebuger::ErrorLog("Not Found XD Config Json");
		return nullptr;
	}
}

TSharedPtr<XUType::Config> FXULocalConfig::GenerateXUConfig() {
	TSharedPtr<XUType::Config> ConfigPtr = MakeShareable(new XUType::Config);
	if (region_type == "global") {
		ConfigPtr->RegionType = XUType::Global;
	} else {
		ConfigPtr->RegionType = XUType::CN;
	}

	ConfigPtr->ClientId = client_id;
	ConfigPtr->AppID = app_id;
	// ConfigPtr->IdfaEnabled = idfa_enabled;
	ConfigPtr->GameName = game_name;

	// tap sdk
	ConfigPtr->LogoutUrl = logout_url;
	ConfigPtr->ReportUrl = report_url;
	ConfigPtr->WebPayUrl = webpay_url;
	ConfigPtr->TapConfig.ClientID = tapsdk.client_id;
	ConfigPtr->TapConfig.ClientToken = tapsdk.client_token;
	ConfigPtr->TapConfig.ServerURL = tapsdk.server_url;
	ConfigPtr->TapConfig.RegionType = ConfigPtr->RegionType == XUType::CN ? TUType::CN : TUType::Global;
	ConfigPtr->TapLoginPermissions = tapsdk.permissions;
	// tap db
	ConfigPtr->TapConfig.DBConfig.Enable = tapsdk.db_config.enable;
	ConfigPtr->TapConfig.DBConfig.Channel = tapsdk.db_config.channel;
	ConfigPtr->TapConfig.DBConfig.GameVersion = tapsdk.db_config.game_version;
	// ConfigPtr->TapConfig.DBConfig.AdvertiserIDCollectionEnabled = idfa_enabled;
	
	// ConfigPtr->FacebookInfo.AppID = facebook.app_id;
	// ConfigPtr->FacebookInfo.ClientToken = facebook.client_token;
	// ConfigPtr->FacebookLoginPersmissions = facebook.permissions;
	//
	// ConfigPtr->LineInfo.ChannelID = line.channel_id;
	//
	// ConfigPtr->TwitterInfo.ConsumerKey = twitter.consumer_key;
	// ConfigPtr->TwitterInfo.ConsumerSecret = twitter.consumer_secret;

	ConfigPtr->GoogleInfo.ClientID = google.CLIENT_ID_FOR_PC;
	ConfigPtr->AppleInfo.ServerID = apple.service_id;

	// for (auto Event : adjust.events) {
	// 	XUType::AdjustEvent Temp;
	// 	Temp.Token = Event.token;
	// 	Temp.EventName = Event.event_name;
	// 	ConfigPtr->AdjustInfo.Events.Add(Temp);
	// }
	// ConfigPtr->AdjustInfo.AppToken = adjust.app_token;
	//
	// ConfigPtr->AppsflyerInfo.DevKey = appsflyer.dev_key;
	// ConfigPtr->AppsflyerInfo.AppID = appsflyer.app_id;

	if (ConfigPtr->RegionType == XUType::CN) {
		ConfigPtr->Agreement.Url = "https://protocol.xd.cn/sdk/merger.html";
		ConfigPtr->Agreement.Version = "latest";
		ConfigPtr->Agreement.Region = "DF";
		ConfigPtr->Agreement.IsKRPushServiceSwitchEnable = false;
	}
	
	return ConfigPtr;
}

// void FXULocalConfig::SetLocalConfigJsonName(const FString& ConfigName) {
// 	ConfigJsonName = ConfigName;
// }
