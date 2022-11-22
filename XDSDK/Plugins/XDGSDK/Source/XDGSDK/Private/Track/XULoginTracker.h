#pragma once
#include "XUError.h"

class XULoginTracker {
public:

	static FString GetCurrentSessionId();

	static void ClearCurrentSessionId();
	
	// 开始登录
	static void LoginStart();

	// 登录封控拦截成功
	static void LoginRiskSuccess(FXUError Error);
	
	// 跳转授权
	static void Login2Authorize();
	
	// 授权成功
	static void Login2AuthorizeSuccess();
	
	// 授权失败
	static void Login2AuthorizeFailed(const FString& Reason);
	
	// SDK 登陆成功
	static void LoginPreLoginSuccess();
	
	// SDK 登录失败
	static void LoginPreLoginFailed(const FString& Reason);

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
	
};
