#pragma once
#include "TUType.h"

namespace TULoginType {
	
	class Config {
	public:
		FString ClientID;
		TUType::RegionType RegionType = TUType::CN;
		bool RoundCorner = true;
		TUType::LanguageType LanguageType = TUType::AUTO;
	};
	
};
