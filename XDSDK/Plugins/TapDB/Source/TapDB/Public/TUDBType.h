#pragma once

#include "TUType.h"


namespace TUDBType {

	class Config {
	public:
		// 注册游戏时获得的APP ID
		FString ClientId;

		// 分包渠道名称，可为空
		FString Channel;

		// 游戏版本
		FString GameVersion;

		// 是否是国内
		TUType::RegionType RegionType;
	};

	class LoginType {
	public:
		static FString TapTap;
		static FString WeiXin;
		static FString QQ;
		static FString Tourist;
		static FString Apple;
		static FString ZhiFuBao;
		static FString Facebook;
		static FString Google;
		static FString Twitter;
		static FString PhoneNumber;
	};
	
}
