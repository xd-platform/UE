#pragma once
#include "TapUser.h"
#include "TUError.h"
#include "TUType.h"

class TAPBOOTSTRAP_API TapUEBootstrap {
public:

	// 初始化 
	static void Init(const TUType::Config& Config);

	// 仅支持移动端 
	static void Login(TArray<FString> Permissions, TFunction<void(const FTapUser& User)> SuccessBlock, TFunction<void(const FTUError& Error)> FailBlock);

	// 仅支持移动端 
	static void AnonymouslyLogin(TFunction<void(const FTapUser& User)> SuccessBlock, TFunction<void(const FTUError& Error)> FailBlock);

	// 仅支持移动端 
	static void Logout();

	// 仅支持移动端 
	static TSharedPtr<FTapUser> GetUser();

	// 仅支持移动端 
	static void SetPreferLanguage(TUType::LanguageType LangType);

	// static void GetDetailUser();
	//
	// static void GetAccessToken();
	//
	// static void OpenUserCenter();
	//
	// static void GetTestQualification();
	
};
