// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Core.h"
#include "CoreMinimal.h"
#include "XDGCommonBridge.h"

#import <Foundation/Foundation.h>
#import <XDCommonSDK/XDGSDK.h>
#import <XDCommonSDK/XDGTrackerManager.h>
#import <XDCommonSDK/XDGSDKSettings.h>
#import <XDCommonSDK/NSDictionary+TDSGlobalJson.h>
#import <XDCommonSDK/XDGCoreService.h>
#import <XDCommonSDK/XDConfigManager.h>

#import <AppTrackingTransparency/AppTrackingTransparency.h>
#import <AdSupport/AdSupport.h>
#import <XDCommonSDK/TDSGlobalDeviceInfoTool.h>

#pragma clang diagnostic ignored "-Wobjc-property-no-attribute"
#pragma clang diagnostic ignored "-Wundef"
#import <TapCommonSDK/TapCommonSDK.h>


class  XDGCommonIOS : public XDGCommonBridge
{
public:
	XDGCommonIOS();
	~XDGCommonIOS();

	FString GetSDKVersionName();
	void SetLanguage(int32 langType);
	void InitSDK();
	bool IsInitialized();
	void Report(FString serverId, FString roleId, FString roleName);
	void StoreReview();

	void ShareFlavors(int32 type, FString uri, FString message);
	void ShareImage(int32 type, FString imagePath);

	void TrackUser(FString userId);
	void TrackRole(FString serverId, FString roleId, FString roleName, int32 level);
	void TrackEvent(FString eventName);

	void SetCurrentUserPushServiceEnable(bool enable);
	bool IsCurrentUserPushServiceEnable();

	void GetRegionInfo();
	void GetXDGInfoJson();

    void DisableAgreementUI();
	void SetCountryRegion(FString region);

	void DevelopInit(int32 num);
	
	virtual void ShowDetailAgreement(FString Url) override;
	virtual TArray<FXDGAgreement> GetAgreementList() override;
	
	virtual void ClearAllUserDefaultsData() override;
	void TrackAchievement();
	void EventCompletedTutorial();
	void EventCreateRole();

};

@interface XDGUE4CommonTool : NSObject
+ (void)shareWithResult:(NSError *)error cancel:(BOOL)cancel;
@end
