#pragma once
#include "XUError.h"
#include "XUType.h"

class XULoginTracker {
public:

	static FString GetCurrentSessionId();

	static void ClearCurrentSessionId();
	
	// 开始登录
	static void LoginStart(const FString& LoginTypeString);

	// 登录封控拦截成功
	static void LoginRiskSuccess(FXUError Error);
	
	// 跳转授权
	static void Login2Authorize(bool IsSilent = false);
	
	// 授权成功
	static void Login2AuthorizeSuccess(bool IsSilent = false);
	
	// 授权失败
	static void Login2AuthorizeFailed(const FString& Reason, bool IsSilent = false);
	
	// SDK 登陆成功
	static void LoginPreLoginSuccess(bool IsSilent = false);
	
	// SDK 登录失败
	static void LoginPreLoginFailed(const FString& Reason, bool IsSilent = false);

	// 登陆成功
	static void LoginSuccess();
	
	// 登录失败
	static void LoginFailed(const FString& Reason);
	
	// 退出登录
	static void Logout(const FString& Reason);

private:
	static void LogEvent(const FString& EventName, TSharedPtr<FJsonObject> Properties);
	static TSharedPtr<FJsonObject> GetCommonProperties();

	static FString EventSessionId;
	static FString LoginType;

};
