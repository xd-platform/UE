#include "TULoginPCImpl.h"

#include "TAULoginLanguage.h"
#include "Server/TULoginNet.h"
#include "UI/TAULoginWidget.h"

void TULoginPCImpl::Init(TULoginType::Config _Config) {
	this->Config = _Config;
}

void TULoginPCImpl::ChangeLanguage(TUType::LanguageType LanguageType) {
	switch (LanguageType) {
	case TUType::AUTO:
		if (Config.RegionType == TUType::CN) {
			TAULoginLanguage::SetLangType(TAULoginLanguage::CN);
		}
		else {
			TAULoginLanguage::SetLangType(TAULoginLanguage::EN);
		}
		break;
	case TUType::ZH:
		TAULoginLanguage::SetLangType(TAULoginLanguage::CN);
		break;
	case TUType::EN:
		TAULoginLanguage::SetLangType(TAULoginLanguage::EN);
		break;
	case TUType::ZHTW:
		TAULoginLanguage::SetLangType(TAULoginLanguage::ZHTW);
		break;
	case TUType::JA:
		TAULoginLanguage::SetLangType(TAULoginLanguage::JA);
		break;
	case TUType::KO:
		TAULoginLanguage::SetLangType(TAULoginLanguage::KO);
		break;
	case TUType::TH:
		TAULoginLanguage::SetLangType(TAULoginLanguage::TH);
		break;
	case TUType::ID:
		TAULoginLanguage::SetLangType(TAULoginLanguage::ID);
		break;
	default: ;
	}
}

TSharedPtr<FTULoginProfileModel> TULoginPCImpl::GetProfile() {
	return FTULoginProfileModel::GetLocalModel();
}

void TULoginPCImpl::FetchProfile(
	TFunction<void(TSharedPtr<FTULoginProfileModel> ModelPtr, const FTUError& Error)> CallBack) {

	auto AccessTokenPtr = GetAccessToken();
	if (!AccessTokenPtr.IsValid() && CallBack != nullptr) {
		CallBack(nullptr, FTUError(-1, "no login"));
		return;
	}
	TULoginNet::RequestProfile(*AccessTokenPtr.Get(), [=](TSharedPtr<FTULoginProfileModel> Model, FTULoginError Error) {
		if (CallBack) {
			CallBack(Model, FTUError(Error.code, Error.msg));
		}
	});
}

TSharedPtr<FTUAccessToken> TULoginPCImpl::GetAccessToken() {
	return FTUAccessToken::GetLocalModel();
}

void TULoginPCImpl::Login(TArray<FString> Permissions, TFunction<void(const TUAuthResult& Result)> CallBack) {
	UTAULoginWidget::ShowLoginUI(Permissions, CallBack);
}

void TULoginPCImpl::Logout() {
	FTULoginProfileModel::ClearLocalModel();
	FTUAccessToken::ClearLocalModel();
}

void TULoginPCImpl::GetTestQualification(TFunction<void(bool IsQualified, const FTUError& Error)> CallBack) {
	if (!FTUAccessToken::GetLocalModel().IsValid()) {
		if (CallBack) {
			CallBack(false, FTUError(-400, "AccessToken is nil."));
		}
		return;
	}
	TULoginNet::RequestTestQualification([=](TSharedPtr<FTUTestQualificationModel> Model, FTULoginError Error) {
		if (!CallBack) {
			return;
		}
		if (Model.IsValid()) {
			CallBack(Model->in_test, FTUError(Error.code, Error.msg));
		} else {
			CallBack(false, FTUError(Error.code, Error.msg));
		}
	});
}
