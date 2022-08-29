// Copyright Epic Games, Inc. All Rights Reserved.

#include "XDGAccount.h"

#define LOCTEXT_NAMESPACE "FXDGAccountModule"

FXDGAccountModule::FXDGSDKLoginSucceed FXDGAccountModule::OnXDGSDKLoginSucceed;
FXDGAccountModule::FXDGSDKLoginFailed FXDGAccountModule::OnXDGSDKLoginFailed;
FXDGAccountModule::FXDGSDKGetUserSucceed FXDGAccountModule::OnXDGSDKGetUserSucceed;
FXDGAccountModule::FXDGSDKGetUserFailed FXDGAccountModule::OnXDGSDKGetUserFailed;
FXDGAccountModule::FXDGSDKUserStateChanged FXDGAccountModule::OnXDGSDKUserStateChanged;
FXDGAccountModule::FXDGSDKBindByTypeCompleted FXDGAccountModule::OnXDGSDKBindByTypeCompleted;

IMPLEMENT_MODULE(FXDGAccountModule, XDGAccount)
void FXDGAccountModule::StartupModule()
{

}

void FXDGAccountModule::ShutdownModule()
{
	
}
#undef LOCTEXT_NAMESPACE
	
