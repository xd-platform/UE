#include "TUBootStrapPCImpl.h"

#include "TapUELogin.h"
#include "TUHelper.h"
#include "TULoginType.h"

void TUBootStrapPCImpl::Init(const TUType::Config& Config) {
	TUType::Config::Init(Config);
	
	// 初始化 TapLogin
	TULoginType::Config LoginConfig;
	LoginConfig.ClientID = Config.ClientID;
	LoginConfig.RegionType = Config.RegionType;
	TapUELogin::Init(LoginConfig);

	// 初始化 TapDB
	if (Config.DBConfig.Enable) {
		TUHelper::InvokeNoReturnFunction("TUDBReflection", "Init");
	}
	
}

void TUBootStrapPCImpl::Login(TArray<FString> Permissions, TFunction<void(const FTapUser& User)> SuccessBlock,
	TFunction<void(const FTUError& Error)> FailBlock) {
	TUBootStrapImpl::Login(Permissions, SuccessBlock, FailBlock);
}

void TUBootStrapPCImpl::AnonymouslyLogin(TFunction<void(const FTapUser& User)> SuccessBlock,
	TFunction<void(const FTUError& Error)> FailBlock) {
	TUBootStrapImpl::AnonymouslyLogin(SuccessBlock, FailBlock);
}

void TUBootStrapPCImpl::Logout() {
	TapUELogin::Logout();
}

TSharedPtr<FTapUser> TUBootStrapPCImpl::GetUser() {
	return TUBootStrapImpl::GetUser();
}

void TUBootStrapPCImpl::SetPreferLanguage(TUType::LanguageType LangType) {
	TapUELogin::ChangeLanguage(LangType);
}
