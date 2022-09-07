#pragma once
#include "TUHttpRequest.h"

class XUTrackNet: public TUHttpRequest {
public:
	XUTrackNet();

	static void UploadTrackInfo(TSharedPtr<FJsonObject> Info, TFunction<void(TSharedPtr<TUHttpResponse> Response)> Callback);

	static void UploadTrackInfos(TArray<TSharedPtr<FJsonObject>> Infos, TFunction<void(TSharedPtr<TUHttpResponse> Response)> Callback);
};
