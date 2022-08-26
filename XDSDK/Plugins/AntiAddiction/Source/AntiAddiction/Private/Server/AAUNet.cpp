#include "AAUNet.h"

#include "AAUImpl.h"
#include "AntiAddiction.h"
#include "TUCrypto.h"
#include "TUDebuger.h"
#include "TUDeviceInfo.h"
#include "TUHttpManager.h"
#include "TUJsonHelper.h"
#include "TUOpenSSL.h"
#include "Model/AAUUser.h"


bool AAUNet::RealNameServerIsCrash = false;

FTUError GenerateErrorInfo(const TSharedPtr<TUHttpResponse>& Response)
{
	FTUError Error = FTUError();
	Error.code = Response->state;
	Error.error_description = TEXT("网络异常，请稍后重试");
	return Error;
}


void JudgeServerIsCrash(const TSharedPtr<TUHttpResponse>& Response) {
	if (Response->state == TUHttpResponse::serverError) {
		AAUNet::RealNameServerIsCrash = true;
	} else if (Response->contentString.IsEmpty() && Response->state != TUHttpResponse::networkError) {
		AAUNet::RealNameServerIsCrash = true;
	}
}

template <typename StructType>
void PerfromResponseCallBack(const TSharedPtr<TUHttpResponse>& Response,
							 TFunction<void(TSharedPtr<StructType> ModelPtr, const FTUError& Error)> Callback) {
	if (Callback == nullptr) {
		return;
	}
	FTUError Error = GenerateErrorInfo(Response);

	auto JsonObject = TUJsonHelper::GetUStruct<StructType>(Response->contentString);

	Callback(JsonObject, Error);
}

template <typename StructType>
void GenerateWrapperResponse(const TSharedPtr<TUHttpResponse>& Response, TSharedPtr<StructType>& ModelPtr, FTUError& Error)
{
	Error = GenerateErrorInfo(Response);

	if (Response->state == TUHttpResponse::serverError) {
		return;
	}

	const auto JsonObject = TUJsonHelper::GetJsonObject(Response->contentString);
	bool Success = false;
	const TSharedPtr<FJsonObject>* DataJsonObject = nullptr;
	if (JsonObject.IsValid() && JsonObject->TryGetBoolField("success", Success) && JsonObject->TryGetObjectField("data", DataJsonObject))
	{
		if (Success)
		{
			ModelPtr = TUJsonHelper::GetUStruct<StructType>(*DataJsonObject);
		} else
		{
			const auto ErrorPtr = TUJsonHelper::GetUStruct<FTUError>(*DataJsonObject);
			if (ErrorPtr.IsValid())
			{
				Error = *ErrorPtr.Get();
			}
		}
	}
}

template <typename StructType>
void PerfromWrapperResponseCallBack(const TSharedPtr<TUHttpResponse>& Response, TFunction<void(TSharedPtr<StructType> ModelPtr, const FTUError& Error)> Callback)
{
	if (Callback == nullptr)
	{
		return;
	}
	FTUError Error;
	TSharedPtr<StructType> ModelPtr = nullptr;
	GenerateWrapperResponse(Response, ModelPtr, Error);
	Callback(ModelPtr, Error);
}


static FString TDSBaseUrl = "https://tds-tapsdk.cn.tapapis.com";  // TDS的域名
static FString TapBaseUrl = "https://openapi.taptap.com";			// TapTap的域名



static FString RealNameAuthenticationPath = "/real-name/v1/";  // 实名认证部分
static FString AntiAddictionPath = "/anti-addiction/v1/";      // 防沉迷部分

static FString TDSPublicConfigUrl = "https://tds-public-config-sh.oss-cn-shanghai.aliyuncs.com";    // 拉取配置阿里云的地址

AAUNet::AAUNet() {
	TimeoutSecs = 15;
	Form = Json;
}

/// 获取防沉迷静态配置，目前只有一个stand_alone_mode字段
void AAUNet::GetStaticSetting(TFunction<void(TSharedPtr<FAAUSettingsModel> ModelPtr, const FTUError& Error)> CallBack) {
	const TSharedPtr<AAUNet> request = MakeShareable(new AAUNet());
	request->URL = TDSPublicConfigUrl / "/antiaddiction-settings.json";
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		PerfromResponseCallBack(response, CallBack);
	});
	TUHttpManager::Get().request(request);
}

void AAUNet::GetServerTime(TFunction<void(TSharedPtr<FAAUServerTimeModel> ModelPtr, const FTUError& Error)> CallBack) {
	const TSharedPtr<AAUNet> request = MakeShareable(new AAUNet());
	request->URL = TDSBaseUrl / AntiAddictionPath / "server-time";
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		PerfromWrapperResponseCallBack(response, CallBack);
	});
	TUHttpManager::Get().request(request);
}

void AAUNet::GetSDKConfig(TFunction<void(TSharedPtr<FAAUConfigModel> ModelPtr, const FTUError& Error)> CallBack) {
	const TSharedPtr<AAUNet> request = MakeShareable(new AAUNet());
	request->URL = TDSBaseUrl / AntiAddictionPath / "clients/{clients}/configuration" ;
	request->PathParameters.Add("{clients}", AAUImpl::Get()->Config.ClientID);
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		PerfromWrapperResponseCallBack(response, CallBack);
	});
	TUHttpManager::Get().request(request);
}

void AAUNet::SetPayment(int Amount,
	TFunction<void(TSharedPtr<FAAUPaymentModel> ModelPtr, const FTUError& Error)> CallBack) {
	const TSharedPtr<AAUNet> request = MakeShareable(new AAUNet());
	request->URL = TDSBaseUrl / AntiAddictionPath / "clients/{clients}/users/{users}/payments";
	request->PathParameters.Add("{clients}", AAUImpl::Get()->Config.ClientID);
	request->PathParameters.Add("{users}", FAAUUser::GetCurrentUser()->UserID);
	request->Type = Post;
	request->Headers.Add("Authorization", FAAUUser::GetCurrentUser()->AccessToken);
	request->Parameters->SetNumberField("amount", Amount);
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		PerfromWrapperResponseCallBack(response, CallBack);
	});
	TUHttpManager::Get().request(request);
}

void AAUNet::CheckPayment(int Amount,
	TFunction<void(TSharedPtr<FAAUPayableModel> ModelPtr, const FTUError& Error)> CallBack) {
	const TSharedPtr<AAUNet> request = MakeShareable(new AAUNet());
	request->URL = TDSBaseUrl / AntiAddictionPath / "clients/{clients}/users/{users}/payable";
	request->PathParameters.Add("{clients}", AAUImpl::Get()->Config.ClientID);
	request->PathParameters.Add("{users}", FAAUUser::GetCurrentUser()->UserID);
	request->Type = Post;
	request->Headers.Add("Authorization", FAAUUser::GetCurrentUser()->AccessToken);
	request->Parameters->SetNumberField("amount", Amount);
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		PerfromWrapperResponseCallBack(response, CallBack);
	});
	TUHttpManager::Get().request(request);
}

void AAUNet::CheckPlayable(const FString& UserID, const FString& Token, TArray<TArray<int>> ServerTimes,
                           TArray<TArray<int>> LocalTimes,
                           TFunction<void(TSharedPtr<FAAUPlayableModel> ModelPtr, const FTUError& Error)> CallBack, bool IsLogin) {
	const TSharedPtr<AAUNet> request = MakeShareable(new AAUNet());
	request->URL = TDSBaseUrl / AntiAddictionPath / "clients/{clients}/users/{users}/playable";
	request->PathParameters.Add("{clients}", AAUImpl::Get()->Config.ClientID);
	request->PathParameters.Add("{users}", UserID);
	request->Type = Post;
	request->RepeatCount = 3;
	request->Headers.Add("Authorization", Token);

	FString JsonStr;
	TSharedRef <TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&JsonStr);
	JsonWriter->WriteObjectStart();
	JsonWriter->WriteValue("game", AAUImpl::Get()->Config.ClientID);
	const FString Version = AntiAddictionUE_VERSION;
	JsonWriter->WriteValue("sdkVersion", Version);
	JsonWriter->WriteObjectStart("play_logs");
	JsonWriter->WriteArrayStart("server_times");
	for (auto ServerTime : ServerTimes) {
		JsonWriter->WriteArrayStart();
		for (auto Time : ServerTime) {
			JsonWriter->WriteValue(Time);
		}
		JsonWriter->WriteArrayEnd();
	}
	JsonWriter->WriteArrayEnd();
	JsonWriter->WriteArrayStart("local_times");
	for (auto LocalTime : LocalTimes) {
		JsonWriter->WriteArrayStart();
		for (auto Time : LocalTime) {
			JsonWriter->WriteValue(Time);
		}
		JsonWriter->WriteArrayEnd();
	}
	JsonWriter->WriteArrayEnd();
	JsonWriter->WriteObjectEnd();
	JsonWriter->WriteValue("is_login", IsLogin ? 1 : 0);
	JsonWriter->WriteObjectEnd(); 
	JsonWriter->Close();

	request->PostBodyString = JsonStr;
	auto object = TUJsonHelper::GetJsonObject(JsonStr);
	JsonStr = TUJsonHelper::GetJsonString(object);
	
	// TSharedPtr<FJsonObject> PlayLogsJsonObject = MakeShareable(new FJsonObject);
	// PlayLogsJsonObject->SetArrayField("server_times", ServerTimes);
	
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		JudgeServerIsCrash(response);
		PerfromWrapperResponseCallBack(response, CallBack);
	});
	TUHttpManager::Get().request(request);
}



void AAUNet::ManualVerify(const FString& UserID, const FString& Name, const FString& CardID,
                          TFunction<void(TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error)> CallBack) {
	const TSharedPtr<AAUNet> request = MakeShareable(new AAUNet());
	request->URL = TDSBaseUrl / RealNameAuthenticationPath / "clients/{clients}/users/{users}/manual" ;
	request->PathParameters.Add("{clients}", AAUImpl::Get()->Config.ClientID);
	request->PathParameters.Add("{users}", UserID);
	request->Type = Post;
	request->RepeatCount = 3;
	
	TSharedPtr<FJsonObject> UserInfo = MakeShareable(new FJsonObject);
	UserInfo->SetStringField("name", Name);
	UserInfo->SetStringField("id_card", CardID);
	FString UserInfoStr = TUJsonHelper::GetJsonString(UserInfo);
	FString Key = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA1pM6yfulomBTXWKiQT5gK9fY4hq11Kv8D+ewum25oPGReuEn6dez7ogA8bEyQlnYYUoEp5cxYPBbIxJFy7q1qzQhTFphuFzoC1x7DieTvfZbh+b60psEottrCD8M0Pa3h44pzyIp5U5WRpxRcQ9iULolGLHZXJr9nW6bpOsyEIFG5tQ7qCBj8HSFoNBKZH+5Cwh3j5cjmyg55WdJTimg9ysbbwZHYmI+TFPuGo/ckHT6j4TQLCmmxI8Qf5pycn3/qJWFhjx/y8zaxgn2hgxbma8hyyGRCMnhM5tISYQv4zlQF+5RashvKa2zv+FHA5DALzIsGXONeTxk6TSBalX5gQIDAQAB";
	if (TUDebuger::IsTest) {
		for (auto Replace : TUDebuger::ReplaceOtherContents) {
			if (Key.Contains(Replace.Key)) {
				Key.ReplaceInline(*Replace.Key, *Replace.Value);
				break;
			}
		}
	}

	auto EncryptData = TUOpenSSL::RSAEncryptPublic(TUCrypto::UTF8Encode(UserInfoStr), Key);
	request->Parameters->SetStringField("data", TUCrypto::Base64Encode(EncryptData));
	
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		JudgeServerIsCrash(response);
		FTUError Error;
		TSharedPtr<FAAURealNameResultModel> ModelPtr = nullptr;
		GenerateWrapperResponse(response, ModelPtr, Error);
		if (CallBack) {
			CallBack(ModelPtr, Error);
		}
	});
	TUHttpManager::Get().request(request);
}

void AAUNet::CheckRealNameState(const FString& UserID,
	TFunction<void(TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error)> CallBack) {
	const TSharedPtr<AAUNet> request = MakeShareable(new AAUNet());
	request->URL = TDSBaseUrl / RealNameAuthenticationPath / "clients/{clients}/users/{users}";
	request->PathParameters.Add("{clients}", AAUImpl::Get()->Config.ClientID);
	request->PathParameters.Add("{users}", UserID);
	request->Type = Get;
	request->RepeatCount = 3;
	
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		JudgeServerIsCrash(response);
		FTUError Error;
		TSharedPtr<FAAURealNameResultModel> ModelPtr = nullptr;
		GenerateWrapperResponse(response, ModelPtr, Error);
		if (!ModelPtr.IsValid()) {
			if (response->state == TUHttpResponse::networkError) {
				Error.error_description = TEXT("网络异常，请稍后重试");
			}
			else {
				Error.error_description = TEXT("未查询到实名状态，防沉迷启动失败");
			}
		}
		if (CallBack) {
			CallBack(ModelPtr, Error);
		}
	});
	TUHttpManager::Get().request(request);
}

// "/v3/fcm/authorizations"  // POST



TMap<FString, FString> AAUNet::CommonHeaders() {
	TMap<FString, FString> HeadMap = TUHttpRequest::CommonHeaders();
	FString UAStr = FString::Printf(TEXT("Version=%s&Platform=%s&Unreal-SDK-Version=%s"), TEXT(AntiAddictionUE_VERSION), *TUDeviceInfo::GetPlatform(), *TUDeviceInfo::GetEngineVersion());
	HeadMap.Add("UA", UAStr);
	return HeadMap;
	
	// [request setValue:[requestParams objectForKey:ANTI_AUTH_KEY] forHTTPHeaderField:ANTI_AUTH_KEY]; Authorization
}

TSharedPtr<FJsonObject> AAUNet::CommonParameters() {
	auto _Parameters = TUHttpRequest::CommonParameters();
	_Parameters->SetStringField("game", AAUImpl::Get()->Config.ClientID);
	_Parameters->SetStringField("sdkVersion", AntiAddictionUE_VERSION);
	return _Parameters;
}

bool AAUNet::ResetHeadersBeforeRequest() {
	return TUHttpRequest::ResetHeadersBeforeRequest();
}


