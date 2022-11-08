#include "TUMomentImpl.h"
#include "TUDebuger.h"
#if PLATFORM_MAC || PLATFORM_WINDOWS
#include "Desktop/TUMomentPCImpl.h"
#elif PLATFORM_IOS || PLATFORM_ANDROID
#include "Mobile/TUMomentMobileImpl.h"
#endif

#define UnsupportedPlatformsLog TUDebuger::ErrorLog(FString::Printf(TEXT("Unsupported Platforms Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));

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
	UnsupportedPlatformsLog
}

void TUMomentImpl::SetCallback(TFunction<void(int Code, const FString& Msg)> CallBack) {
	UnsupportedPlatformsLog
}

void TUMomentImpl::Open(TUMomentType::Orientation Orientation) {
	UnsupportedPlatformsLog
}

void TUMomentImpl::Publish(TUMomentType::Orientation Orientation, const TArray<FString>& ImagePaths,
	const FString& Content) {
	UnsupportedPlatformsLog
}

void TUMomentImpl::PublishVideo(TUMomentType::Orientation Orientation, const TArray<FString>& VideoPaths,
	const TArray<FString>& ImagePaths, const FString& Title, const FString& Desc) {
	UnsupportedPlatformsLog
}

void TUMomentImpl::PublishVideo(TUMomentType::Orientation Orientation, const TArray<FString>& VideoPaths,
	const FString& Title, const FString& Desc) {
	UnsupportedPlatformsLog
}

void TUMomentImpl::FetchNotification() {
	UnsupportedPlatformsLog
}

void TUMomentImpl::Close() {
	UnsupportedPlatformsLog
}

void TUMomentImpl::Close(const FString& Title, const FString& Content) {
	UnsupportedPlatformsLog
}

// void TUMomentImpl::SetUseAutoRotate(bool UseAuto) {
// 	UnsupportedPlatformsLog
// }

void TUMomentImpl::DirectlyOpen(TUMomentType::Orientation Orientation, const FString& Page,
	TSharedPtr<FJsonObject> Extras) {
	UnsupportedPlatformsLog
}

void TUMomentImpl::NeedDeferSystemGestures() {
	UnsupportedPlatformsLog
}

void TUMomentImpl::OpenWebTopic() {
	UnsupportedPlatformsLog
}
