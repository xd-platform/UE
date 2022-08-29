// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <string>
#include "Modules/ModuleManager.h"
#include "CoreUObject.h"
#include "Engine.h"

class XDGACCOUNT_API FXDGAccountModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

    static inline FXDGAccountModule& Get(){
        return FModuleManager::LoadModuleChecked<FXDGAccountModule>( "XDGAccount" );
    }

    static inline bool IsAvailable(){
        return FModuleManager::Get().IsModuleLoaded( "XDGAccount" );
    } 


    DECLARE_MULTICAST_DELEGATE_OneParam(FXDGSDKLoginSucceed, const FString&);
	DECLARE_MULTICAST_DELEGATE_ThreeParams(FXDGSDKLoginFailed, const int32, const FString&, const FString&);
    DECLARE_MULTICAST_DELEGATE_OneParam(FXDGSDKGetUserSucceed, const FString&);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FXDGSDKGetUserFailed, const int32, const FString&);
    DECLARE_MULTICAST_DELEGATE_TwoParams(FXDGSDKUserStateChanged, const int32, const FString&);
     DECLARE_MULTICAST_DELEGATE_ThreeParams(FXDGSDKBindByTypeCompleted, const bool, const int32, const FString&);
   
    UPROPERTY(BlueprintAssignable, Category = "XDGAccount")
    static FXDGSDKLoginSucceed OnXDGSDKLoginSucceed;

	UPROPERTY(BlueprintAssignable, Category = "XDGAccount")
    static FXDGSDKLoginFailed OnXDGSDKLoginFailed;

    UPROPERTY(BlueprintAssignable, Category = "XDGAccount")
    static FXDGSDKGetUserSucceed OnXDGSDKGetUserSucceed;

	UPROPERTY(BlueprintAssignable, Category = "XDGAccount")
    static FXDGSDKGetUserFailed OnXDGSDKGetUserFailed;

    UPROPERTY(BlueprintAssignable, Category = "XDGAccount")
    static FXDGSDKUserStateChanged OnXDGSDKUserStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "XDGAccount")
    static FXDGSDKBindByTypeCompleted OnXDGSDKBindByTypeCompleted;
};
