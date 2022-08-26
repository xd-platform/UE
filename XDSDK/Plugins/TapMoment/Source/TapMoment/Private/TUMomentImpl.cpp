#include "TUMomentImpl.h"
#include "TUDebuger.h"
#if PLATFORM_MAC || PLATFORM_WINDOWS
#include "Desktop/TUMomentPCImpl.h"
#elif PLATFORM_IOS || PLATFORM_ANDROID
#include "Mobile/TUMomentMobileImpl.h"
#endif

TSharedPtr<TUMomentImpl> TUMomentImpl::Instance = nullptr;

TSharedPtr<TUMomentImpl>& TUMomentImpl::Get() {
	if (!Instance.IsValid()) {
#if PLATFORM_MAC || PLATFORM_WINDOWS
		Instance = MakeShareable(new TUMomentPCImpl);
#elif PLATFORM_IOS || PLATFORM_ANDROID
		Instance = MakeShareable(new TUMomentMobileImpl);
#else
		Instance = MakeShareable(new TUMomentImpl);
#endif
	}
	return Instance;
}

void TUMomentImpl::Init(const TUMomentType::Config& InitConfig) {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TUMomentImpl::SetCallback(TFunction<void(int Code, const FString& Msg)> CallBack) {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TUMomentImpl::Open(TUMomentType::Orientation Orientation) {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TUMomentImpl::Publish(TUMomentType::Orientation Orientation, const TArray<FString>& ImagePaths,
	const FString& Content) {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TUMomentImpl::PublishVideo(TUMomentType::Orientation Orientation, const TArray<FString>& VideoPaths,
	const TArray<FString>& ImagePaths, const FString& Title, const FString& Desc) {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TUMomentImpl::PublishVideo(TUMomentType::Orientation Orientation, const TArray<FString>& VideoPaths,
	const FString& Title, const FString& Desc) {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TUMomentImpl::FetchNotification() {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TUMomentImpl::Close() {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TUMomentImpl::Close(const FString& Title, const FString& Content) {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

// void TUMomentImpl::SetUseAutoRotate(bool UseAuto) {
// 	TUDebuger::ErrorLog("Unsupported Platforms");
// }

void TUMomentImpl::DirectlyOpen(TUMomentType::Orientation Orientation, const FString& Page,
	TSharedPtr<FJsonObject> Extras) {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TUMomentImpl::NeedDeferSystemGestures() {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TUMomentImpl::OpenWebTopic() {
	TUDebuger::ErrorLog("Unsupported Platforms");
}
