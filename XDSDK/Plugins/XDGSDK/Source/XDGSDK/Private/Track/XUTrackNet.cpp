#include "XUTrackNet.h"

#include "TUHttpManager.h"
#include "TUJsonHelper.h"
#include "XURegionConfig.h"

XUTrackNet::XUTrackNet() {
	Form = Json;
	Type = Post;
	Headers.Add("x-log-apiversion", "0.6.0");
}

void XUTrackNet::UploadTrackInfo(TSharedPtr<FJsonObject> Info,
	TFunction<void(TSharedPtr<TUHttpResponse> Response)> Callback) {
	UploadTrackInfos({Info},Callback);
}

void XUTrackNet::UploadTrackInfos(TArray<TSharedPtr<FJsonObject>> Infos,
	TFunction<void(TSharedPtr<TUHttpResponse> Response)> Callback) {
	const TSharedPtr<TUHttpRequest> request = MakeShareable(new XUTrackNet());
	TArray<TSharedPtr<FJsonValue>> Contents;
	request->URL = XURegionConfig::Get()->TrackUrl();
	for (auto JsonObject : Infos) {
		FString InfoStr = TUJsonHelper::GetJsonString(JsonObject);
		TSharedPtr<FJsonObject> TempJsonObject = MakeShareable(new FJsonObject);
		TempJsonObject->SetStringField("content", InfoStr);
		Contents.Add( MakeShareable(new FJsonValueObject(TempJsonObject)) );
	}
	request->Parameters->SetArrayField("__logs__", Contents);
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> Response) {
		if (Callback) {
			Callback(Response);
		}
	});
	TUHttpManager::Get().request(request);
}
