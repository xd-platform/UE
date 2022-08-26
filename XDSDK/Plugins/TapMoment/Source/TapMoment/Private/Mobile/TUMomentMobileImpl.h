#pragma once
#include "TUMomentImpl.h"

class TUMomentMobileImpl : public TUMomentImpl {
public:
	TUMomentMobileImpl();
	
	virtual void Init(const TUMomentType::Config& InitConfig) override;
	virtual void SetCallback(TFunction<void(int Code, const FString& Msg)> CallBack) override;
	virtual void Open(TUMomentType::Orientation Orientation) override;
	virtual void Publish(TUMomentType::Orientation Orientation, const TArray<FString>& ImagePaths,
		const FString& Content) override;
	virtual void PublishVideo(TUMomentType::Orientation Orientation, const TArray<FString>& VideoPaths,
		const TArray<FString>& ImagePaths, const FString& Title, const FString& Desc) override;
	virtual void PublishVideo(TUMomentType::Orientation Orientation, const TArray<FString>& VideoPaths,
		const FString& Title, const FString& Desc) override;
	virtual void FetchNotification() override;
	virtual void Close() override;
	virtual void Close(const FString& Title, const FString& Content) override;
	// virtual void SetUseAutoRotate(bool UseAuto) override;
	virtual void DirectlyOpen(TUMomentType::Orientation Orientation, const FString& Page,
		TSharedPtr<FJsonObject> Extras) override;
	virtual void NeedDeferSystemGestures() override;
};
