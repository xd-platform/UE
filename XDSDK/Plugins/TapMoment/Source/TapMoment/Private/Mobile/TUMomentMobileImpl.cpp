#include "TUMomentMobileImpl.h"

#include "TUJsonHelper.h"
#include "TUMobileBridge.h"

#define TAP_MOMENT_CLZ "com.tapsdk.moment.wrapper.TapMomentService"
#define TAP_MOMENT_IMPL "com.tapsdk.moment.wrapper.TapMomentServiceImpl"
#define TAP_MOMENT_SERVICE "TapMomentService"

TUMomentMobileImpl::TUMomentMobileImpl() {
	TUMobileBridge::Register(TEXT(TAP_MOMENT_CLZ),TEXT(TAP_MOMENT_IMPL));
}

void TUMomentMobileImpl::Init(const TUMomentType::Config& InitConfig) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("clientId"), InitConfig.ClientID);
	Writer->WriteValue(TEXT("regionType"), InitConfig.RegionType == ERegionType::CN);
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TAP_MOMENT_SERVICE, "initWithRegion", JsonOutString);
}

void TUMomentMobileImpl::SetCallback(TFunction<void(int Code, const FString& Msg)> CallBack) {
	TUMobileBridge::AsyncPerform(TAP_MOMENT_SERVICE, "setCallback", "", [=](const FString& ResultJsonStr) {
		if (CallBack == nullptr) {
			return;
		}
		int Code = -1;
		FString Msg = "";
		auto ResultPtr = TUJsonHelper::GetJsonObject(ResultJsonStr);
		if (ResultPtr.IsValid()) {
			ResultPtr->TryGetNumberField("code", Code);
			ResultPtr->TryGetStringField("message", Msg);
		}
		CallBack(Code, Msg);
	}, false);
}

void TUMomentMobileImpl::Open(TUMomentType::Orientation Orientation) {
	FString args;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&args);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("config"), (int)Orientation);
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TAP_MOMENT_SERVICE, "open", args);
}

void TUMomentMobileImpl::Publish(TUMomentType::Orientation Orientation, const TArray<FString>& ImagePaths,
	const FString& Content) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("imagePaths"), ImagePaths);
	Writer->WriteValue(TEXT("config"), (int)Orientation);
	Writer->WriteValue(TEXT("content"), Content);
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TAP_MOMENT_SERVICE, "publish", JsonOutString);
}

void TUMomentMobileImpl::PublishVideo(TUMomentType::Orientation Orientation, const TArray<FString>& VideoPaths,
	const TArray<FString>& ImagePaths, const FString& Title, const FString& Desc) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("imagePaths"), ImagePaths);
	Writer->WriteValue(TEXT("videoPaths"), VideoPaths);
	Writer->WriteValue(TEXT("config"), (int)Orientation);
	Writer->WriteValue(TEXT("title"), Title);
	Writer->WriteValue(TEXT("desc"), Desc);
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TAP_MOMENT_SERVICE, "publishVideoImage", JsonOutString);
}

void TUMomentMobileImpl::PublishVideo(TUMomentType::Orientation Orientation, const TArray<FString>& VideoPaths,
	const FString& Title, const FString& Desc) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("videoPaths"), VideoPaths);
	Writer->WriteValue(TEXT("config"), (int)Orientation);
	Writer->WriteValue(TEXT("title"), Title);
	Writer->WriteValue(TEXT("desc"), Desc);
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TAP_MOMENT_SERVICE, "publishVideo", JsonOutString);
}

void TUMomentMobileImpl::FetchNotification() {
	TUMobileBridge::AsyncPerform(TAP_MOMENT_SERVICE, "fetchNotification", "");
}

void TUMomentMobileImpl::Close() {
	TUMobileBridge::AsyncPerform(TAP_MOMENT_SERVICE, "close", "");
}

void TUMomentMobileImpl::Close(const FString& Title, const FString& Content) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("title"), Title);
	Writer->WriteValue(TEXT("content"), Content);
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TAP_MOMENT_SERVICE, "closeWithConfirmWindow", JsonOutString);
}

// void TUMomentMobileImpl::SetUseAutoRotate(bool UseAuto) {
// 	TUMomentImpl::SetUseAutoRotate(UseAuto);
// }

void TUMomentMobileImpl::DirectlyOpen(TUMomentType::Orientation Orientation, const FString& Page,
	TSharedPtr<FJsonObject> Extras) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("config"), (int)Orientation);
	Writer->WriteValue(TEXT("page"), Page);
	Writer->WriteValue(TEXT("extras"), TUJsonHelper::GetJsonString(Extras));
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TAP_MOMENT_SERVICE, "directlyOpen", JsonOutString);
}

void TUMomentMobileImpl::NeedDeferSystemGestures() {
#if PLATFORM_IOS
	TUMobileBridge::AsyncPerform(TAP_MOMENT_SERVICE, "needDeferSystemGestures", "");
#else
	TUMomentImpl::NeedDeferSystemGestures();
#endif
}
