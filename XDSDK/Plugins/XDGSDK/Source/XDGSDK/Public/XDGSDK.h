// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#define XDUESDK_VERSION_NUMBER "60600001"
#define XDUESDK_VERSION "6.6.0"


class XDGSDK_API FXDGSDKModule : public IModuleInterface
{
public:
	
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
