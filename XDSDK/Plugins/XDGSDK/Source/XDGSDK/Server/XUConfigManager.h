#pragma once
#include "XUIpInfoModel.h"
#include "XUType.h"

struct FXUServerConfig;
typedef TFunction<void(TSharedPtr<XUType::Config> Config, const FString& Msg)> XUConfigHandler;

class XUConfigManager {
public:
	FString TargetRegion;
	bool AgreementUIEnable;
	FString ConfigFileName;

	static XUConfigManager& SharedInstance();
	
	static TSharedPtr<XUType::Config> CurrentConfig();

	static void SetConfig(TSharedPtr<XUType::Config> Config);

	static bool IsCN();

	static void ReadLocalConfig(XUConfigHandler Handler);

	static void UpdateConfigWithCache();
	
	static void RequestServerConfig();

	static void InitTapSDK();
	
	// static void UpdateBindEntriesConfig:(NSArray *)config;

	// static void UpdateHttpConfig();

	// + (XDGRegionInfo *)GetRegionInfo();

	static void GetRegionInfo(TFunction<void(TSharedPtr<FXUIpInfoModel> ModelPtr)> ResultBlock);

	static bool IsGameInited();

	static bool GoogleEnable();
	// static bool FacebookEnable();
	static bool TapTapEnable();
	static bool TapDBEnable();
	// static bool AdjustEnable();
	// static bool AppsflyersEnable();
	// static bool LineEnable();
	// static bool TwitterEnable();

	static bool NeedReportService();
	static bool IsGameInKoreaAndPushServiceEnable();
	static bool IsGameInNA();
	
	static void RecordKRPushSetting(bool PushOn);

	static bool GetKRPushSetting();

private:
	
	static XUConfigManager * Instance;
	TSharedPtr<XUType::Config> Config;
	
	static void UpdateConfig(TSharedPtr<FXUServerConfig> ServerConfig);
};
