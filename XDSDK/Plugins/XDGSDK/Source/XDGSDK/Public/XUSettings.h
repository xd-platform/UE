#pragma once
#include "XUType.h"

class XDGSDK_API XUSettings {
public:
	/// 设置调试模式，debug 会输出SDK日志
	/// @param Debug 是否 debug 模式。默认 NO
	static void SetDebugMode(bool Debug);

	/// 设置SDK显示语言
	/// @param Locale 语言，在 XUType::LangType 枚举中查看
	static void SetLanguage(XUType::LangType Locale);

	/// 设置SDK期望地区
	/// @param TargetCountryOrRegion 期望地区
	static void SetTargetCountryOrRegion(const FString& TargetCountryOrRegion);

	/// 设置是否打开SDK的协议窗口
	/// @param Enable 是否打开，默认YES
	static void SetAgreementUIEnable(bool Enable);

	/// 更新初始化文件名
	/// @param FileName 初始化文件名
	static void UpdateConfigFileName(const FString& FileName);

	/// 设置Widget控件的层级。
	/// @param Level Widget层级
	static void SetUILevel(int32 Level);

	/// 设置当前游戏的GameInstance。
	/// @param GameInstance 游戏当前GameInstance
	static void SetGameInstance(TWeakObjectPtr<UGameInstance> GameInstance);

	// 设置打开外部浏览器前的回调（用于解决打开外部浏览器出现意外的状态，比如全屏游戏会覆盖浏览器）
	static void SetBlockBeforeLaunchUrl(TFunction<void()> Block);
	
};
