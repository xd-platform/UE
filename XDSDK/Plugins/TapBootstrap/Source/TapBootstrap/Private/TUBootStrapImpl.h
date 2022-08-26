#pragma once
#include "TapUser.h"
#include "TUError.h"
#include "TUType.h"

class TUBootStrapImpl {
public:

	virtual ~TUBootStrapImpl() = default;

	static TSharedPtr<TUBootStrapImpl>& Get();
	
	virtual void Init(const TUType::Config& Config);

	virtual void Login(TArray<FString> Permissions, TFunction<void(const FTapUser& User)> SuccessBlock,
		TFunction<void(const FTUError& Error)> FailBlock);

	virtual void AnonymouslyLogin(TFunction<void(const FTapUser& User)> SuccessBlock,
		TFunction<void(const FTUError& Error)> FailBlock);

	virtual void Logout();

	virtual TSharedPtr<FTapUser> GetUser();

	virtual void SetPreferLanguage(TUType::LanguageType LangType);

private:
	static TSharedPtr<TUBootStrapImpl> Instance;
};
