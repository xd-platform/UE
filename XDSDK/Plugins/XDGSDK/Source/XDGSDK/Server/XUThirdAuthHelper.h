#pragma once
#include "TUAccessToken.h"
#include "XUError.h"

class XUThirdAuthHelper {
public:
	enum WebAuthType {
		GoogleAuth,
		AppleAuth,
	};

	static void TapTapAuth(TFunction<void(FTUAccessToken AccessToken)> Callback, TFunction<void(FXUError Error)> ErrorBlock);

	static void WebAuth(WebAuthType AuthType, TFunction<void(TSharedPtr<FJsonObject> AuthParas)> Callback, TFunction<void(FXUError Error)> ErrorBlock);

	static void CancelAllPreAuths();

private:
	static TArray<FString> CacheStates;

	static FString GenerateState();

	static bool JudgeContainStateAndClearAllAuths(FString State);
};
