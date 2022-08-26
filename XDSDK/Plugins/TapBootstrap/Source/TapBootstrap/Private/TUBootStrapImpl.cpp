#include "TUBootStrapImpl.h"
#include "TUDebuger.h"
#if PLATFORM_MAC || PLATFORM_WINDOWS
#include "Desktop/TUBootStrapPCImpl.h"
#elif PLATFORM_IOS || PLATFORM_ANDROID
#include "Mobile/TUBootStrapMobileImpl.h"
#endif

TSharedPtr<TUBootStrapImpl> TUBootStrapImpl::Instance = nullptr;

TSharedPtr<TUBootStrapImpl>& TUBootStrapImpl::Get() {
	if (!Instance.IsValid()) {
#if PLATFORM_MAC || PLATFORM_WINDOWS
		Instance = MakeShareable(new TUBootStrapPCImpl);
#elif PLATFORM_IOS || PLATFORM_ANDROID
		Instance = MakeShareable(new TUBootStrapMobileImpl);
#else
		Instance = MakeShareable(new TUBootStrapImpl);
#endif
	}
	return Instance;
}

void TUBootStrapImpl::Init(const TUType::Config& Config) {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TUBootStrapImpl::Login(TArray<FString> Permissions, TFunction<void(const FTapUser& User)> SuccessBlock,
	TFunction<void(const FTUError& Error)> FailBlock) {
	TUDebuger::ErrorLog("Unsupported Platforms");
	// if (FailBlock) {
	// 	FailBlock(FTUError(FTUError::ERROR_CODE_LOGIN_Platforms_Unsupported, "Unsupported Platforms"));
	// }
}

void TUBootStrapImpl::AnonymouslyLogin(TFunction<void(const FTapUser& User)> SuccessBlock,
	TFunction<void(const FTUError& Error)> FailBlock) {
	TUDebuger::ErrorLog("Unsupported Platforms");
	// if (FailBlock) {
	// 	FailBlock(FTUError(FTUError::ERROR_CODE_LOGIN_Platforms_Unsupported, "Unsupported Platforms"));
	// }
}

void TUBootStrapImpl::Logout() {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

TSharedPtr<FTapUser> TUBootStrapImpl::GetUser() {
	TUDebuger::ErrorLog("Unsupported Platforms");
	return nullptr;
}

void TUBootStrapImpl::SetPreferLanguage(TUType::LanguageType LangType) {
	TUDebuger::ErrorLog("Unsupported Platforms");
}
