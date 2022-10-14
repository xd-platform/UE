#pragma once
#include "TUType.h"


namespace XUType {
	enum RegionType {
		CN,
		// 国内
		Global // 海外
	};

	enum PayResult {
		PayOK,
		PayFail,
		PayCancel
	};

	enum UserChangeState {
		UserLogout          = 0x9001,   // user logout
		UserBindSuccess     = 0x1001,   // user bind success,msg = entry type in string,eg: @"TAPTAP"
		UserUnBindSuccess   = 0x1002,   // user unbind success,msg = entry type in string
	};

	enum LangType {
		ZH_CN = 0,
		// 中文
		ZH_TW = 1,
		// 繁体中文
		EN = 2,
		// 英文
		TH = 3,
		// 泰文
		ID = 4,
		// 印尼文
		KR = 5,
		// 韩语
		JP = 6,
		// 日语
		DE = 7,
		// 德语
		FR = 8,
		// 法语
		PT = 9,
		// 葡萄牙语
		ES = 10,
		// 西班牙语
		TR = 11,
		// 土耳其语
		RU = 12,
		// 俄罗斯语
		VI = 13,
		// 越南语
	};

	enum LoginType {
		Default = -1,
		// 自动登录
		Guest = 0,
		// 游客登录
		// WeChat = 1,		
		Apple = 2,
		Google = 3,
		// Facebook = 4,		
		TapTap = 5,
		// TapTap登录
		// Line = 6,
		// Twitter = 7,
	};

	enum CheckPayType {
		iOS,
		// 只有iOS需要补款
		Android,
		// 只有Android需要补款
		iOSAndAndroid,
		// iOS Android 都需要补款
		None,
		// 没有要补款
	};

	struct GoogleConfig {
		FString ClientID;
	};

	struct AppleConfig {
		FString ServerID;
	};

	struct AgreementConfig {
		FString Url;
		FString Version;
		FString Region;
		bool IsKRPushServiceSwitchEnable = false;
	};

	struct BindEntriesConfig {
		bool CanBind;
		FString EntryName;
		bool CanUnbind;
	};

	class Config {
	public:
		/// 区域选择，可选 CN、Global
		RegionType RegionType = CN;

		/// XDGSDK client id
		FString ClientId;

		/// TapSDK 配置
		FTUConfig TapConfig;

		/// 游戏对外名称 显示在 Facebook 登录和客服页面中
		FString GameName;

		/// TapTap 授权权限
		TArray<FString> TapLoginPermissions;

		// 游戏版本
		FString GameVersion = "1.0.0";

		/*
		 * 以下内容按需配置
		 */

		/// Google 配置信息
		GoogleConfig GoogleInfo;

		/// Apple 配置信息
		AppleConfig AppleInfo;

		/// XDGSDK app id
		FString AppID;

		/// 用户中心窗口中绑定入口选项
		TArray<BindEntriesConfig> BindEntries;

		/// 发行区域
		FString Region;

		/// 客服链接，默认
		FString ReportUrl;

		/// 注销账户链接，默认
		FString LogoutUrl;

		/// 注销账户链接，默认
		FString WebPayUrl;

		/// 协议内容
		AgreementConfig Agreement;
	};
}
