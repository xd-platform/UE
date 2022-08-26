#pragma once
#include "TUMomentType.h"

class TUMomentImpl {
public:

	TUMomentType::Config Config;

	virtual ~TUMomentImpl() = default;

	static TSharedPtr<TUMomentImpl>& Get();
	
	virtual void Init(const TUMomentType::Config& InitConfig);
	
	virtual void SetCallback(TFunction<void(int Code, const FString& Msg)> CallBack);

	virtual void Open(TUMomentType::Orientation Orientation);

	virtual void Publish(TUMomentType::Orientation Orientation, const TArray<FString>& ImagePaths, const FString& Content);

	virtual void PublishVideo(TUMomentType::Orientation Orientation, const TArray<FString>& VideoPaths,
		const TArray<FString>& ImagePaths, const FString& Title, const FString& Desc);

	virtual void PublishVideo(TUMomentType::Orientation Orientation, const TArray<FString>& VideoPaths, const FString& Title, const FString& Desc);

	virtual void FetchNotification();

	virtual void Close();

	virtual void Close(const FString& Title, const FString& Content);

	// virtual void SetUseAutoRotate(bool UseAuto);

	virtual void DirectlyOpen(TUMomentType::Orientation Orientation, const FString& Page, TSharedPtr<FJsonObject> Extras);

	virtual void NeedDeferSystemGestures();

	virtual void OpenWebTopic();

private:
	static TSharedPtr<TUMomentImpl> Instance;
};
