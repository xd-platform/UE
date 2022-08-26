#pragma once


namespace TUType {

	// 这个影响服务的host以及TapTap授权客户端的种类（是国内还是海外）
	enum RegionType {
		CN,		// 国内
		Global		// 海外
	};

	enum LanguageType {
		AUTO,   // 海外默认英语，国内默认简体中文
		ZH,		// 简体中文
		EN,		// 英文，海外默认语言
		ZHTW,	// 繁体中文
		JA,		// 日语
		KO,		// 韩语
		TH,		// 泰文
		ID,		// 印尼文
	};
	
	class TAPCOMMON_API Config {
	public:

		FString ClientID;

		FString ClientToken;

		RegionType RegionType;

		FString ServerURL;

		static void Init(const Config& InitConfig);
		static TSharedPtr<Config>& Get();
		

		class TapDB {
		public:
			bool Enable; // 如果该值为true，将会自动把TapDB模块一起初始化，不用手动去初始化TapDB
	
			FString Channel;

			FString GameVersion;
			
			bool AdvertiserIDCollectionEnabled;
		};
		
		TapDB DBConfig;
		


	private:
		static TSharedPtr<Config> Instance;
		
	};

	class TAPCOMMON_API PermissionScope {
	public:
		static FString Profile;  // 获得用户信息的权限
		static FString Friend;	 // 获得好友的权限
	};
}
