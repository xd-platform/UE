// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

#define TapUEBootstrap_VERSION_NUMBER "30900001"
#define TapUEBootstrap_VERSION "3.9.0"

class TAPBOOTSTRAP_API FTapBootstrapModule : public IModuleInterface
{
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    
};
