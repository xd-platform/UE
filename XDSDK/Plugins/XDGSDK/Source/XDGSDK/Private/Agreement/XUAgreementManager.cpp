#include "XUAgreementManager.h"

#include "TUDeviceInfo.h"
#include "TUHelper.h"
#include "XDUE.h"
#include "XUConfigManager.h"
#include "XUImpl.h"
#include "XULanguageManager.h"
#include "Track/XUProtocolTracker.h"
#include "Track/XUTracker.h"
#include "XDGSDK/UI/XUPrivacyWidget.h"

static FString const XD_CACHE_AGREEMENT = "XD_CACHE_AGREEMENT";
static FString const XD_CACHE_SERVER_AGREEMENT = "XD_CACHE_SERVER_AGREEMENT";

TSharedPtr<XUAgreementManager> XUAgreementManager::Instance = nullptr;

XUAgreementManager& XUAgreementManager::SharedInstance() {
	if (Instance == nullptr) {
		Instance = MakeShareable(new XUAgreementManager);
	}
	return *Instance.Get();
}

XUAgreementManager::XUAgreementManager() {
	XUImpl::OnLoginSuccess.AddRaw(this, &XUAgreementManager::UploadUserAgreement);
	XUImpl::OnLogoutSuccess.AddRaw(this, &XUAgreementManager::ShowAgreementWhenLogout);
	XUImpl::OnTokenIsInvalid.AddRaw(this, &XUAgreementManager::ShowAgreementWhenLogout);
}

TSharedPtr<FXUAgreementConfig> XUAgreementManager::GetCurrentAgreement() {
	return SharedInstance().CurrentAgreement;
}

void XUAgreementManager::RequestServerAgreementsExceptCN(TFunction<void(bool Success)> Handler) {
	
	// 读取缓存协议
	auto ServerAgreement = TUDataStorage<FXUStorage>::LoadStruct<FXUAgreementConfig>(XD_CACHE_SERVER_AGREEMENT);
	if (ServerAgreement.IsValid() && !ServerAgreement->agreementUrl.IsEmpty()) {
		SharedInstance().CurrentAgreement = ServerAgreement;
	} else {
		// 生成保底协议
		SharedInstance().CurrentAgreement = FXUAgreementConfig::GenerateDefaultConfig(XUConfigManager::IsCN());
	}
	// 海外获取服务端协议
	if (XUConfigManager::IsCN()) {
		if (Handler) {
			Handler(true);
		}
	} else {
		RequestServerAgreements(true, Handler);
	}
}

void XUAgreementManager::RequestServerAgreements(bool InitTime, TFunction<void(bool Success)> Handler) {
	// 请求成功后不再请求
	if (SharedInstance().RequestSuccess) {
		if (Handler) {
			Handler(true);
		}
		return;
	}
	XUNet::RequestAgreement(InitTime, [=](TSharedPtr<FXUAgreementConfig> Model, FXUError Error) {
		if (Model.IsValid() && !Model->agreementUrl.IsEmpty()) {
			SharedInstance().RequestSuccess = true;
			TUDataStorage<FXUStorage>::SaveStruct(XD_CACHE_SERVER_AGREEMENT, Model);
			
			// 更新成服务端协议
			SharedInstance().CurrentAgreement = Model;
			// 国内已经签署的是保底协议版本的话用服务端版本刷新本地缓存
			if (XUConfigManager::IsCN()) {
				auto LocalAgreementDic = TUDataStorage<FXUStorage>::LoadJsonObject(GetRegionAgreementCacheName());
				TSharedPtr<FXUAgreementConfig> AgreementConfig = nullptr;
				if (LocalAgreementDic.IsValid() && LocalAgreementDic->Values.Num() > 0) {
					AgreementConfig = TUJsonHelper::GetUStruct<FXUAgreementConfig>(LocalAgreementDic);
					if (AgreementConfig.IsValid() && !AgreementConfig->agreementVersion.IsEmpty()) {
						if (AgreementConfig->agreementVersion == "latest") {
							if (!Model->agreementVersion.IsEmpty()) {
								AgreementConfig->agreementVersion = Model->agreementVersion;
								SaveAgreementConfig(AgreementConfig, false);
							}
						}
					}
				}
			}
			if (Handler) {
				Handler(true);
			}
		} else {
			if (Handler) {
				Handler(false);
			}
		}
	});
}

void XUAgreementManager::CheckAgreementWithHandler(TFunction<void()> Handler) {
	if (!NeedShowAgreement()) {
		if (Handler) {
			Handler();
		}
		return;
	}
	UXUPrivacyWidget::ShowPrivacy([=]() {
		int32 PrivacyType = 1;
		if (XUConfigManager::IsGameInKoreaAndPushServiceEnable()) {
			PrivacyType = 2;
		} else if (XUConfigManager::IsGameInNA()) {
			PrivacyType = 3;
		}
		XUProtocolTracker::AgreePrivacy(PrivacyType);
		SharedInstance().HasSignedAgreement = true;
		if (Handler) {
			Handler();
		}
	});
	XUProtocolTracker::AskPrivacy();
}

FString XUAgreementManager::GetAgreementUrl() {
	FString BaseUrl = GetCurrentAgreement()->agreementUrl;
	auto LocalAgreementDic = TUDataStorage<FXUStorage>::LoadJsonObject(GetRegionAgreementCacheName());

	TSharedPtr<FJsonObject> Paras = MakeShareable(new FJsonObject);
	Paras->SetStringField("client_id", XUConfigManager::CurrentConfig()->ClientId);
	Paras->SetStringField("language", XULanguageManager::GetLanguageKey());
	if (!XUConfigManager::SharedInstance().TargetRegion.IsEmpty()) {
		Paras->SetStringField("region", XUConfigManager::SharedInstance().TargetRegion);
	}
	if (LocalAgreementDic.IsValid() && LocalAgreementDic->Values.Num() > 0) {
		Paras->SetStringField("firstCheck", "false");
	} else {
		Paras->SetStringField("firstCheck", "true");
	}
	FString Url = BaseUrl + "?" + TUHelper::CombinParameters(Paras);
	return Url;
}

void XUAgreementManager::ResetAgreement() {
	TUDataStorage<FXUStorage>::Remove(GetRegionAgreementCacheName());
	SharedInstance().HasSignedAgreement = false;
}

void XUAgreementManager::UploadUserAgreement() {
	// 没有协议或者当前没签协议时不上传
	if (!CurrentAgreement.IsValid() || CurrentAgreement->agreementVersion.IsEmpty() || !HasSignedAgreement) {
		return;
	}
	
	bool Upload = false;
	auto LocalAgreementDic = TUDataStorage<FXUStorage>::LoadJsonObject(GetRegionAgreementCacheName());
	if (LocalAgreementDic.IsValid() && LocalAgreementDic->Values.Num() > 0) {
		auto localCacheAgreementConfig = TUJsonHelper::GetUStruct<FXUAgreementConfig>(LocalAgreementDic);
		if (!localCacheAgreementConfig->agreementVersion.IsEmpty()) {
			FString LocalAgreementCacheStr = localCacheAgreementConfig->agreementVersion + localCacheAgreementConfig->agreementRegion;
			FString currentAgreementStr = CurrentAgreement->agreementVersion + CurrentAgreement->agreementRegion;
			if (LocalAgreementCacheStr == currentAgreementStr) {
				bool oldUploadSuccess = LocalAgreementDic->GetBoolField("upload");
				if (oldUploadSuccess) {
					// 接口版本和缓存版本一致且已上报过时不再上报
					Upload = true; // 已经上传
				}
			}
		}
	}

	if (!Upload) {
		TSharedPtr<FJsonObject> postData = MakeShareable(new FJsonObject);
		postData->SetStringField("clientId", XUConfigManager::CurrentConfig()->ClientId);
		postData->SetStringField("deviceCode", TUDeviceInfo::GetLoginId());
		postData->SetStringField("agreementVersion", CurrentAgreement->agreementVersion);
		postData->SetStringField("agreementRegion", CurrentAgreement->agreementRegion);
		auto UserInfo = FXUUser::GetLocalModel();
		postData->SetStringField("userId", UserInfo.IsValid() ? UserInfo->userId : "");
		if (XUConfigManager::IsGameInKoreaAndPushServiceEnable() && XUConfigManager::GetKRPushSetting()) {
			TSharedPtr<FJsonObject> ExtraData = MakeShareable(new FJsonObject);
			ExtraData->SetBoolField("push_agreement，value", true);
			postData->SetObjectField("extra", ExtraData);
		}
		XUNet::UploadAgreement(postData, [=](TSharedPtr<FXUUploadAgreementResultModel> Model, FXUError Error) {
			if (Model.IsValid() && Model->isSuccess) {
				SaveAgreementConfig(CurrentAgreement, Model->isSuccess);
			} else {
				SaveAgreementConfig(CurrentAgreement, false);
			}
		});
	}
}

void XUAgreementManager::ShowAgreementWhenLogout() {
	// 清除本地已签署协议缓存
	ResetAgreement();
	CheckAgreementWithHandler([]() {
		// 这里不需要做啥
	});
}

FString XUAgreementManager::GetRegionAgreementCacheName() {
	auto CurrentAgreement = GetCurrentAgreement();
	return XD_CACHE_AGREEMENT + (CurrentAgreement.IsValid() ? CurrentAgreement->agreementRegion : "");
}

void XUAgreementManager::SaveAgreementConfig(TSharedPtr<FXUAgreementConfig> AgreementConfig, bool Upload) {
	if (!AgreementConfig.IsValid() || AgreementConfig->agreementVersion.IsEmpty()) {
		return;
	}
	TSharedPtr<FJsonObject> PostData = MakeShareable(new FJsonObject);
	PostData->SetStringField("clientId", XUConfigManager::CurrentConfig()->ClientId);
	PostData->SetStringField("deviceCode", TUDeviceInfo::GetLoginId());
	PostData->SetStringField("agreementVersion", AgreementConfig->agreementVersion);
	PostData->SetStringField("agreementRegion", AgreementConfig->agreementRegion);
	auto UserInfo = FXUUser::GetLocalModel();;
	PostData->SetStringField("userId", UserInfo.IsValid() ? UserInfo->userId : "");
	PostData->SetBoolField("upload", Upload);
	TUDataStorage<FXUStorage>::SaveJsonObject(GetRegionAgreementCacheName(), PostData);
}

bool XUAgreementManager::NeedShowAgreement() {
	if (!XUConfigManager::CurrentConfig().IsValid()) {
		return false;
	}
	if (XUConfigManager::SharedInstance().AgreementUIEnable == false) {
		return false;
	}
	auto CurrentAgreement = GetCurrentAgreement();
	if (!CurrentAgreement.IsValid() || CurrentAgreement->agreementUrl.IsEmpty()) {
		// 海外config接口拿不到数据时不弹窗
		return false;
	}
	auto localAgreementDic = TUDataStorage<FXUStorage>::LoadJsonObject(GetRegionAgreementCacheName());
	if (localAgreementDic.IsValid() && localAgreementDic->Values.Num() > 0) {
		auto localCacheAgreementConfig = TUJsonHelper::GetUStruct<FXUAgreementConfig>(localAgreementDic); 
		if (localCacheAgreementConfig.IsValid() && !localCacheAgreementConfig->agreementVersion.IsEmpty()) {
			FString localCacheAgreementStr = localCacheAgreementConfig->agreementVersion + localCacheAgreementConfig->agreementRegion;
			FString currentAgreementStr = CurrentAgreement->agreementVersion + CurrentAgreement->agreementRegion;
			if (localCacheAgreementStr != currentAgreementStr) {
				// 接口版本和缓存版本不一致时需要签署
				return true;
			}
		} else {
			// 640 版本数据读出异常时重新签署
			return true;
		}
	} else {
		// 判断老版本数据
		if (FXUServerConfig::CanShowPrivacyAlert()) {
			return true;
		} else {
			SaveAgreementConfig(CurrentAgreement, false);
		}
	}
	return false;
}

