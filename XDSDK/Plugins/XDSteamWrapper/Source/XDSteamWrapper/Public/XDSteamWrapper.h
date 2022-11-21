// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

#define XDSTEAMWRAPPER_VERSION_NUMBER "60800001"
#define XDSTEAMWRAPPER_VERSION "6.8.0"

class FXDSteamWrapperModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
