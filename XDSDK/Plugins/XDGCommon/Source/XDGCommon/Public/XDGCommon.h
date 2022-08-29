// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <string>
#include "Modules/ModuleManager.h"
#include "CoreUObject.h"
#include "Engine.h"

class XDGCOMMON_API FXDGCommonModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	 static inline FXDGCommonModule& Get(){
            return FModuleManager::LoadModuleChecked<FXDGCommonModule>( "XDGCommon" );
      }
    
      static inline bool IsAvailable(){
            return FModuleManager::Get().IsModuleLoaded( "XDGCommon" );
      }

	DECLARE_MULTICAST_DELEGATE_TwoParams(FXDGSDKInitCompleted, const bool, const FString&);
	DECLARE_MULTICAST_DELEGATE_OneParam(FXDGSDKShareCompleted, const int32);
    DECLARE_MULTICAST_DELEGATE_FourParams(FXDGSDKGetRegionInfoCompleted, const FString&, const FString&, const FString&, const FString&);
    DECLARE_MULTICAST_DELEGATE_OneParam(FXDGSDKGetXDGInfoJsonCompleted, const FString&);
   

    UPROPERTY(BlueprintAssignable, Category = "XDGCommon")
    static FXDGSDKInitCompleted OnXDGSDKInitCompleted;

	UPROPERTY(BlueprintAssignable, Category = "XDGCommon")
    static FXDGSDKShareCompleted OnXDGSDKShareCompleted;

    UPROPERTY(BlueprintAssignable, Category = "XDGCommon")
    static FXDGSDKGetRegionInfoCompleted OnXDGSDKGetRegionInfoCompleted;

    UPROPERTY(BlueprintAssignable, Category = "XDGCommon")
    static FXDGSDKGetXDGInfoJsonCompleted OnXDGSDKGetXDGInfoJsonCompleted;

};
