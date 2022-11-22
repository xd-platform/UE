// Copyright Epic Games, Inc. All Rights Reserved.

#include "XDSteamWrapperBPLibrary.h"
#include "OnlineSubsystemSteam.h"
#include "TUDebuger.h"
#include "TUHelper.h"
#include "XUNotification.h"

THIRD_PARTY_INCLUDES_START
#include "steam/steam_api.h"
THIRD_PARTY_INCLUDES_END

class SteamCallbacksListener 
{ 
private: 
	STEAM_CALLBACK(SteamCallbacksListener, onGetAuthSessionTicket, GetAuthSessionTicketResponse_t, mGetAuthSessionTicketListener); 

	SteamCallbacksListener(); 
	~SteamCallbacksListener(); 

public: 
	static SteamCallbacksListener* GetInstance() 
	{ 
		static SteamCallbacksListener instance; 
		return &instance; 
	}

	FString SteamTicket;
}; 
// GetAuthSessionTicketResponse_t
SteamCallbacksListener::SteamCallbacksListener() 
: mGetAuthSessionTicketListener(this, &SteamCallbacksListener::onGetAuthSessionTicket) 
{
} 

SteamCallbacksListener::~SteamCallbacksListener() 
{ 
}

void SteamCallbacksListener::onGetAuthSessionTicket(GetAuthSessionTicketResponse_t* _pParam) {
	GetAuthSessionTicketResponse_t Param = * _pParam;
	TUHelper::PerformOnGameThread([=]() {
		TUDebuger::DisplayLog(FString::Printf(TEXT("m_hAuthTicket: %d"), Param.m_hAuthTicket));
		TUDebuger::DisplayLog(FString::Printf(TEXT("m_eResult: %d"), Param.m_eResult));

		if (Param.m_eResult == k_EResultOK) {
			if (Param.m_hAuthTicket == k_HAuthTicketInvalid || SteamTicket.IsEmpty()) {
				XUNotification::SteamTicketDelegate.ExecuteIfBound(0, "Steam GetAuthSessionTicket Fail");
			}
			else {
				TUDebuger::DisplayLog("Steam AuthToken: " + SteamTicket);
				XUNotification::SteamTicketDelegate.ExecuteIfBound(1, SteamTicket);
			}
		}
		else {
			XUNotification::SteamTicketDelegate.ExecuteIfBound(0, "Steam GetAuthSessionTicket Fail");
		}
	});
} 

UXDSteamWrapperBPLibrary::UXDSteamWrapperBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

bool UXDSteamWrapperBPLibrary::SteamSystemIsEnable() {
	auto SteamOSS = FOnlineSubsystemSteam::Get(STEAM_SUBSYSTEM);
	return (SteamOSS && SteamOSS->IsEnabled());
}

FString UXDSteamWrapperBPLibrary::GetSteamID() {
	auto UserPtr = SteamUser();

	if (UserPtr == nullptr || !SteamUser()->BLoggedOn()) {
		return "";
	}
	FString SteamID = FString::Printf(TEXT("%lld"), UserPtr->GetSteamID().ConvertToUint64());
	TUDebuger::DisplayLog("SteamID: " + SteamID);
	return SteamID;
}

void UXDSteamWrapperBPLibrary::GetAuthSessionTicket() {
	auto UserPtr = SteamUser();
	auto Listener = SteamCallbacksListener::GetInstance();
	if (UserPtr == nullptr) {
		XUNotification::SteamTicketDelegate.ExecuteIfBound(0, "No Steam User");
		return;
	}
	if (!SteamUser()->BLoggedOn()) {
		XUNotification::SteamTicketDelegate.ExecuteIfBound(0, "Steam User Not LogOn");
		return;
	}

	uint8 AuthToken[1024];
	uint32 AuthTokenSize = 0;
	UserPtr->GetAuthSessionTicket(AuthToken, UE_ARRAY_COUNT(AuthToken), &AuthTokenSize);
	Listener->SteamTicket = BytesToHex(AuthToken, AuthTokenSize);
}
