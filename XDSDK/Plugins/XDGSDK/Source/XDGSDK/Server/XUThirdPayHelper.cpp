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
			TFunction<void(XUType::PayResult Result)> ResultCallback = *CallbackMap.Find(WebState);

			if (ResultCallback) {
#if PLATFORM_MAC || PLATFORM_WINDOWS
				TUHelper::ActivateItself();
#endif
				TSharedPtr<FJsonObject> PayParas = MakeShareable(new FJsonObject);
				for (auto QueryParam : Request.QueryParams) {
					PayParas->SetStringField(QueryParam.Key, QueryParam.Value);
					TUDebuger::WarningLog(
						FString::Printf(TEXT("收到 Url Key: %s, Value: %s"), *QueryParam.Key, *QueryParam.Value));
				}

				FString pay_result = Request.QueryParams.FindRef("pay_result");
				TUDebuger::WarningLog(FString::Printf(TEXT("pay_result 值: %s"), *pay_result));

				if ("success" == pay_result) {
					ResultCallback(XUType::PayResult::PaySuccess);
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
		FString FinalUrl = PayUrl + "&redirect_uri=" + RedirectUri + "&state=" + State;
		if (TUDebuger::IsTest) {
			for (auto Replace : TUDebuger::ReplaceHosts) {
				if (FinalUrl.Contains(Replace.Key)) {
					FinalUrl.ReplaceInline(*Replace.Key, *Replace.Value);
					break;
				}
			}
		}

		TUDebuger::WarningLog(FString::Printf(TEXT("打开Url: %s"), *FinalUrl));
		UTapCommonBPLibrary::LaunchURL(*FinalUrl, nullptr, nullptr);
	}
}

FString XUThirdPayHelper::GenerateState(TFunction<void(XUType::PayResult Result)> Callback) {
	FString State = FGuid::NewGuid().ToString();
	CallbackMap.Add(State, Callback);
	return State;
}
