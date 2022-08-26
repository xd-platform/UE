#pragma once

namespace AAUType {

	enum Region {
		China,			// 国内            
		Vietnam,		// 越南                 
	};

	enum AgeLimit {
		Child = 0,                   
		Teen = 8,                   
		Young = 16,                        
		Adult = 18,                      
	};

	class Config {
	public:
		// 注册获得的Client ID
		FString ClientID;

		// 是否使用TapTap的快速实名认证(暂不支持)
		bool UseTapLogin = false;

		// 是否支持切换账号
		bool ShowSwitchAccount = false;

		// 国家区域，默认中国
		Region Region = China;
	};

}
