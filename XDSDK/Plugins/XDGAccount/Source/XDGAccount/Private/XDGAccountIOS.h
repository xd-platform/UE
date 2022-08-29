// Fill out your copyright notice in the Description page of Project Settings.
#if PLATFORM_IOS
#pragma once

#include "Core.h"
#include "CoreMinimal.h"
#include "XDGAccountBridge.h"

#import <Foundation/Foundation.h>
#import <XDAccountSDK/XDGAccount.h>
#import <XDCommonSDK/XDGUser.h>
#import <XDCommonSDK/XDGAccessToken.h>
#import <XDCommonSDK/NSDictionary+TDSGlobalJson.h>
#import <XDCommonSDK/XDGThirdTokenManager.h>

/**
 * 
 */
class XDGACCOUNT_API XDGAccountIOS : public XDGAccountBridge
{
public:
	XDGAccountIOS();
	~XDGAccountIOS();

	void Login(FString loginTypeList);
	void LoginByType(FString loginType);
	void AddUserStatusChangeCallback();

    void GetUser();
	void OpenUserCenter();
	void Logout();
	void OpenAccountCancellation();

    bool IsTokenActiveWithType(FString loginType);    
	void BindByType(FString loginType); 
};

@interface XDGUE4AccountTool : NSObject
typedef void (^VoidTapCallback)(void);
+ (NSArray*)getLoginTypeList:(NSString*)listJson;
+ (void)enableTap:(VoidTapCallback) block;
+ (void)bridgeUserCallback:(XDGUser *)user error:(NSError *)error isLogin: (Boolean)isLogin;
@end

#endif 