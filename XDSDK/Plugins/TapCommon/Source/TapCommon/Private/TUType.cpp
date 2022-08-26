#include "TUType.h"

namespace TUType {

	TSharedPtr<Config> Config::Instance = nullptr;

	void Config::Init(const Config& InitConfig) {
		Instance = MakeShareable(new Config(InitConfig));
	}

	TSharedPtr<Config>& Config::Get() {
		if (!Instance.IsValid()) {
			Instance = MakeShareable(new Config);
		}
		return Instance;
	}

	FString PermissionScope::Profile = "public_profile";
	FString PermissionScope::Friend = "user_friends";
}


