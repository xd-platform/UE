// Copyright Epic Games, Inc. All Rights Reserved.

#include "XDGSDK.h"
#include "Modules/ModuleManager.h"
#include "OnlineSubsystemNames.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemSteam.h"
// #include "SteamSharedModule.h"
#include "SocketSubsystemModule.h"

#define LOCTEXT_NAMESPACE "FXDGSDKModule"

void FXDGSDKModule::StartupModule()
{
	// FOnlineSubsystemSteam* OnlineSteamSubsystem = static_cast<FOnlineSubsystemSteam*>(IOnlineSubsystem::Get(STEAM_SUBSYSTEM));
	// // FSteamSharedModule& SharedModule = FSteamSharedModule::Get();
	// const bool bIsNotEditor = (IsRunningDedicatedServer() || IsRunningGame());
	// const bool bSteamOSSEnabled = (OnlineSteamSubsystem && OnlineSteamSubsystem->IsEnabled());

	// Load the Steam modules before first call to API
	// if (SharedModule.AreSteamDllsLoaded() && bIsNotEditor && bSteamOSSEnabled)
	
}

void FXDGSDKModule::ShutdownModule()
{
	// FModuleManager& ModuleManager = FModuleManager::Get();
	//
	// if (ModuleManager.IsModuleLoaded("Sockets"))
	// {
	// 	FSocketSubsystemModule& SSS = FModuleManager::GetModuleChecked<FSocketSubsystemModule>("Sockets");
	// 	// SSS.UnregisterSocketSubsystem(STEAM_SOCKETS_SUBSYSTEM);
	// }
	// FSteamSocketsSubsystem::Destroy();
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FXDGSDKModule, XDGSDK)