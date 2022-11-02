// Fill out your copyright notice in the Description page of Project Settings.


#include "XDGCommonBPLibrary.h"
#include "XDGCommonBridge.h"
#include "Engine.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

UXDGCommonBPLibrary::UXDGCommonBPLibrary(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
}


FString UXDGCommonBPLibrary::GetSDKVersionName(){
    return GetXDGCommonBridge()->GetSDKVersionName();
}

void UXDGCommonBPLibrary::SetLanguage(int32 langType){
    GetXDGCommonBridge()->SetLanguage(langType);
}

void UXDGCommonBPLibrary::InitSDK(){
    GetXDGCommonBridge()->InitSDK();
}

bool UXDGCommonBPLibrary::IsInitialized(){
    return GetXDGCommonBridge()->IsInitialized();
}

void UXDGCommonBPLibrary::ShowDetailAgreement(FString Url) {
    GetXDGCommonBridge()->ShowDetailAgreement(Url);
}

TArray<FXDGAgreement> UXDGCommonBPLibrary::GetAgreementList() {
    return GetXDGCommonBridge()->GetAgreementList();
}

void UXDGCommonBPLibrary::Report(FString serverId, FString roleId, FString roleName){
    GetXDGCommonBridge()->Report(serverId, roleId, roleName);
}

void UXDGCommonBPLibrary::StoreReview(){
    GetXDGCommonBridge()->StoreReview();
}


void UXDGCommonBPLibrary::ShareFlavors(int32 type, FString uri, FString message){
    GetXDGCommonBridge()->ShareFlavors(type, uri, message);
}

void UXDGCommonBPLibrary::ShareImage(int32 type, FString imagePath){
    GetXDGCommonBridge()->ShareImage(type, imagePath);
}


void UXDGCommonBPLibrary::TrackUser(FString userId){
    GetXDGCommonBridge()->TrackUser(userId);
}

void UXDGCommonBPLibrary::TrackUser() {
    GetXDGCommonBridge()->TrackUser();
}

void UXDGCommonBPLibrary::TrackRole(FString serverId, FString roleId, FString roleName, int32 level){
    GetXDGCommonBridge()->TrackRole(serverId, roleId, roleName, level);
}

void UXDGCommonBPLibrary::TrackEvent(FString eventName){
    GetXDGCommonBridge()->TrackEvent(eventName);
}




void UXDGCommonBPLibrary::SetCurrentUserPushServiceEnable(bool enable){
    GetXDGCommonBridge()->SetCurrentUserPushServiceEnable(enable);
}

bool UXDGCommonBPLibrary::IsCurrentUserPushServiceEnable(){
   return GetXDGCommonBridge()->IsCurrentUserPushServiceEnable();
}




void UXDGCommonBPLibrary::GetRegionInfo(){
    GetXDGCommonBridge()->GetRegionInfo();
}

void UXDGCommonBPLibrary::GetXDGInfoJson(){
    GetXDGCommonBridge()->GetXDGInfoJson();
}



void UXDGCommonBPLibrary::DisableAgreementUI(){
    GetXDGCommonBridge()->DisableAgreementUI();
}

void UXDGCommonBPLibrary::SetCountryRegion(FString region){
    GetXDGCommonBridge()->SetCountryRegion(region);
}


 void UXDGCommonBPLibrary::DevelopInit(int32 num){
    GetXDGCommonBridge()->DevelopInit(num);
 }

void UXDGCommonBPLibrary::TrackAchievement(){
    GetXDGCommonBridge()->TrackAchievement();
}

void UXDGCommonBPLibrary::EventCompletedTutorial(){
    GetXDGCommonBridge()->EventCompletedTutorial();
}

void UXDGCommonBPLibrary::EventCreateRole(){
    GetXDGCommonBridge()->EventCreateRole();
}

#if !UE_BUILD_SHIPPING
// only test

void UXDGCommonBPLibrary::ClearAllUserDefaultsData() {
    GetXDGCommonBridge()->ClearAllUserDefaultsData();
}

#endif