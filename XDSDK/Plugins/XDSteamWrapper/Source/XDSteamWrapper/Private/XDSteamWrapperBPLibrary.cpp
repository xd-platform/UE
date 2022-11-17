// Copyright Epic Games, Inc. All Rights Reserved.

#include "XDSteamWrapperBPLibrary.h"
#include "XDSteamWrapper.h"
#include "OnlineSubsystemSteam.h"
#include "TUDebuger.h"
#include "Interfaces/OnlineIdentityInterface.h"

UXDSteamWrapperBPLibrary::UXDSteamWrapperBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

bool UXDSteamWrapperBPLibrary::SteamSystemIsEnable() {
	auto SteamOSS = FOnlineSubsystemSteam::Get(STEAM_SUBSYSTEM);
	return (SteamOSS && SteamOSS->IsEnabled());
}

FString UXDSteamWrapperBPLibrary::GetSteamID() {
	auto SteamOSS = FOnlineSubsystemSteam::Get(STEAM_SUBSYSTEM);
	FString SteamID = SteamOSS->GetIdentityInterface()->GetUniquePlayerId(0)->ToString();
	TUDebuger::DisplayShow("SteamID: " + SteamID);
	return SteamID;
}

FString UXDSteamWrapperBPLibrary::GetAuthSessionTicket() {
	auto SteamOSS = FOnlineSubsystemSteam::Get(STEAM_SUBSYSTEM);
	FString AuthToken = SteamOSS->GetIdentityInterface()->GetAuthToken(0);
	TUDebuger::DisplayShow("Steam AuthToken: " + AuthToken);
	return AuthToken;
}
