#include "XUThirdAuthHelper.h"

#include "HttpServerResponse.h"
#include "TapCommonBPLibrary.h"
#include "TapUELogin.h"
#include "TUCrypto.h"
#include "TUDeviceInfo.h"
#include "TUHelper.h"
#include "TUHttpServer.h"
#include "XUConfigManager.h"
#include "XULanguageManager.h"
#include "XURegionConfig.h"


// 同一批Auth放在CacheStates，一旦有一个授权成功了，那么清空CacheStates，也可以手动清空（比如在用户中心关闭弹窗的时候）
TArray<FString> XUThirdAuthHelper::CacheStates;

void XUThirdAuthHelper::TapTapAuth(TFunction<void(FTUAccessToken AccessToken)> Callback,
                                TFunction<void(FXUError Error)> ErrorBlock) {
	FString CurrentState = GenerateState();
	auto Permissons = XUConfigManager::CurrentConfig()->TapLoginPermissions;
	TapUELogin::Login(Permissons, 
		[=](TUAuthResult Result) {
			if (!JudgeContainStateAndClearAllAuths(CurrentState)) {
				return;
			}
			if (Result.GetType() == TUAuthResult::Success) {
				Callback(*Result.GetToken().Get());
			}
			else if (Result.GetType() == TUAuthResult::Cancel) {
				FXUError Error;
				Error.msg = "Login Cancel";
				Error.code = FTUError::ERROR_CODE_LOGIN_CANCEL;
				ErrorBlock(Error);
			}
			else if (Result.GetType() == TUAuthResult::Fail) {
				FXUError Error;
				Error.msg = Result.GetError()->error_description;
				Error.code = Result.GetError()->code;
				ErrorBlock(Error);
			}
			else {
				ErrorBlock(FXUError("Login Fail"));
			}
		});
}

void XUThirdAuthHelper::WebAuth(WebAuthType AuthType, TFunction<void(TSharedPtr<FJsonObject> AuthParas)> Callback,
	TFunction<void(FXUError Error)> ErrorBlock) {
	static FString RedirectUri = "";
	FString RouteName = "";
	if (AuthType == GoogleAuth) {
		RouteName = "google_auth";
	} else if (AuthType == AppleAuth) {
		RouteName = "apple_auth";
	} else {
		return;
	}
	RedirectUri = TUHttpServer::RegisterNewRoute(RouteName, [=](const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete) {
		FString WebState = Request.QueryParams.FindRef("state");
		if (!JudgeContainStateAndClearAllAuths(WebState)) {
			return false;
		}

		if (Callback) {
#if PLATFORM_MAC || PLATFORM_WINDOWS
	TUHelper::ActivateItself();
#endif
			TSharedPtr<FJsonObject> AuthParas = MakeShareable(new FJsonObject);
			for (auto QueryParam : Request.QueryParams) {
				if (QueryParam.Key != "state") {
					AuthParas->SetStringField(QueryParam.Key, QueryParam.Value);
				}
			}
			Callback(AuthParas);
		}
		
		TUniquePtr<FHttpServerResponse> ResponsePtr = MakeUnique<FHttpServerResponse>();
		ResponsePtr->Code = EHttpServerResponseCodes::Ok;
		ResponsePtr->Headers.Add("Content-Type", {"text/plain"});
		ResponsePtr->Headers.Add("Access-Control-Allow-Origin", {"*"});
		ResponsePtr->Body.Append(TUCrypto::UTF8Encode(FString("OK")));
		OnComplete(MoveTemp(ResponsePtr));
		return true;
	});
	if (RedirectUri.IsEmpty()) {
		if (ErrorBlock) {
			ErrorBlock(FXUError("XD Auth Server Create Error"));
		}
	} else {
		FString State = GenerateState();
		
		TSharedPtr<FJsonObject> Paras = MakeShareable(new FJsonObject);
		Paras->SetStringField("redirect_uri", RedirectUri);
		Paras->SetStringField("game_name", XUConfigManager::CurrentConfig()->GameName);
		Paras->SetStringField("lang", XULanguageManager::GetLanguageKey());
		Paras->SetStringField("state", State);
		Paras->SetStringField("from", "UE_" + TUDeviceInfo::GetPlatform());
		if (AuthType == GoogleAuth) {
			FString ClientID = XUConfigManager::CurrentConfig()->GoogleInfo.ClientID;
			Paras->SetStringField("client_id", ClientID);
			Paras->SetStringField("auth_type", "google");
		} else if (AuthType == AppleAuth) {
			FString ClientID = XUConfigManager::CurrentConfig()->AppleInfo.ServerID;
			Paras->SetStringField("client_id", ClientID);
			Paras->SetStringField("auth_type", "apple");
		}
		
		FString ParaStr = TUHelper::CombinParameters(Paras);
		FString URL = XURegionConfig::Get()->LoginWebHost() + "?" + ParaStr;

		URL = TUDebuger::GetReplacedUrl(URL);
		
		// FString URL = "https://xd-website.oss-cn-beijing.aliyuncs.com/xd-order-sgp/v1.0-dev/test/index.html?" + ParaStr;
		TUHelper::LaunchURL(*URL, nullptr, nullptr);
	}
}

void XUThirdAuthHelper::CancelAllPreAuths() {
	CacheStates.Empty();
}

FString XUThirdAuthHelper::GenerateState() {
	FString State = FGuid::NewGuid().ToString();
	CacheStates.Add(State);
	return State;
}

bool XUThirdAuthHelper::JudgeContainStateAndClearAllAuths(FString State) {
	if (CacheStates.Contains(State)) {
		CancelAllPreAuths();
		return true;
	}
	return false;
}
