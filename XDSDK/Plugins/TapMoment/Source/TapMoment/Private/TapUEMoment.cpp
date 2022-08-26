#include "TapUEMoment.h"
#include "TUMomentImpl.h"

void TapUEMoment::Init(TUMomentType::Config Config) {
	TUMomentImpl::Get()->Init(Config);
}

void TapUEMoment::SetCallback(TFunction<void(int Code, const FString& Msg)> CallBack) {
	TUMomentImpl::Get()->SetCallback(CallBack);
}

void TapUEMoment::Open(TUMomentType::Orientation Orientation) {
	TUMomentImpl::Get()->Open(Orientation);
}

void TapUEMoment::Publish(TUMomentType::Orientation Orientation, const TArray<FString>& ImagePaths,
	const FString& Content) {
	TUMomentImpl::Get()->Publish(Orientation, ImagePaths, Content);
}

void TapUEMoment::PublishVideo(TUMomentType::Orientation Orientation, const TArray<FString>& VideoPaths,
	const TArray<FString>& ImagePaths, const FString& Title, const FString& Desc) {
	TUMomentImpl::Get()->PublishVideo(Orientation, VideoPaths, ImagePaths, Title, Desc);
}

void TapUEMoment::PublishVideo(TUMomentType::Orientation Orientation, const TArray<FString>& VideoPaths,
	const FString& Title, const FString& Desc) {
	TUMomentImpl::Get()->PublishVideo(Orientation, VideoPaths, Title, Desc);
}

void TapUEMoment::FetchNotification() {
	TUMomentImpl::Get()->FetchNotification();
}

void TapUEMoment::Close() {
	TUMomentImpl::Get()->Close();
}

void TapUEMoment::Close(const FString& Title, const FString& Content) {
	TUMomentImpl::Get()->Close(Title, Content);
}

// void TapUEMoment::SetUseAutoRotate(bool UseAuto) {
// 	TUMomentImpl::Get()->SetUseAutoRotate(UseAuto);
// }

void TapUEMoment::DirectlyOpen(TUMomentType::Orientation Orientation, const FString& Page,
	TSharedPtr<FJsonObject> Extras) {
	TUMomentImpl::Get()->DirectlyOpen(Orientation, Page, Extras);
}

void TapUEMoment::NeedDeferSystemGestures() {
	TUMomentImpl::Get()->NeedDeferSystemGestures();
}

void TapUEMoment::OpenWebTopic() {
	TUMomentImpl::Get()->OpenWebTopic();
}
