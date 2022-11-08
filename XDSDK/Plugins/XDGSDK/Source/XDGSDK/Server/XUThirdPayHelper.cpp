#include "XUThirdPayHelper.h"

#include "HttpServerConstants.h"
#include "TUCrypto.h"
#include "TUDeviceInfo.h"
#include "TUHelper.h"
#include "TUHttpServer.h"
#include "XUConfigManager.h"
#include "XULanguageManager.h"
#include "XURegionConfig.h"
#include "HttpServerResponse.h"
#include "TapCommonBPLibrary.h"


TMap<FString, TFunction<void(XUType::PayResult Result)>> XUThirdPayHelper::CallbackMap;

void XUThirdPayHelper::StartWebPay(FString PayUrl, TFunction<void(XUType::PayResult Result)> Callback) {
	static FString RedirectUri = "";

	RedirectUri = TUHttpServer::RegisterNewRoute(
		"web_pay", [=](const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete) {
			FString WebState = Request.QueryParams.FindRef("state");
			TFunction<void(XUType::PayResult Result)> *ResultCallbackPtr = CallbackMap.Find(WebState);
			if (ResultCallbackPtr == nullptr) {
				return false;
			}
#if PLATFORM_MAC || PLATFORM_WINDOWS
	TUHelper::ActivateItself();
#endif
			auto ResultCallback = *ResultCallbackPtr;
			if (ResultCallback) {

				FString pay_result = Request.QueryParams.FindRef("pay_result");

				if ("success" == pay_result) {
					ResultCallback(XUType::PayResult::PayOK);
				}
				else if ("cancel" == pay_result) {
					ResultCallback(XUType::PayResult::PayCancel);
				}
				else {
					//fail
					ResultCallback(XUType::PayResult::PayFail);
				}
			}
			else {
				TUDebuger::WarningLog(FString::Printf(TEXT("没找到对应Callback: %s"), *WebState));
			}
			CallbackMap.Remove(WebState);

			TUniquePtr<FHttpServerResponse> ResponsePtr = MakeUnique<FHttpServerResponse>();
			ResponsePtr->Code = EHttpServerResponseCodes::Ok;
			ResponsePtr->Headers.Add("Content-Type", {"text/plain"});
			ResponsePtr->Headers.Add("Access-Control-Allow-Origin", {"*"});
			ResponsePtr->Body.Append(TUCrypto::UTF8Encode(FString("OK")));
			OnComplete(MoveTemp(ResponsePtr));
			return true;
		});

	if (RedirectUri.IsEmpty()) {
		Callback(XUType::PayResult::PayFail);
	}
	else {
		FString State = GenerateState(Callback); //唯一字符串

		TSharedPtr<FJsonObject> Paras = MakeShareable(new FJsonObject);
		Paras->SetStringField("redirect_uri", RedirectUri);
		Paras->SetStringField("state", State);
		FString ParaStr = TUHelper::CombinParameters(Paras);
		FString FinalUrl = PayUrl + "&" + ParaStr;

		FinalUrl = TUDebuger::GetReplacedUrl(FinalUrl);

		TUDebuger::WarningLog(FString::Printf(TEXT("打开Url: %s"), *FinalUrl));
		TUHelper::LaunchURL(*FinalUrl, nullptr, nullptr);
	}
}

FString XUThirdPayHelper::GenerateState(TFunction<void(XUType::PayResult Result)> Callback) {
	FString State = FGuid::NewGuid().ToString();
	CallbackMap.Add(State, Callback);
	return State;
}
