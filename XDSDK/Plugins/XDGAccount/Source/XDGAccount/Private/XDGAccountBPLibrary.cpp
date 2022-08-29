// Fill out your copyright notice in the Description page of Project Settings.


#include "XDGAccountBPLibrary.h"
#include "XDGAccountBridge.h"
#include "Engine.h"

#include "TapCommon.h"
#include "TapJson.h"
#include "TapBridge.h"


UXDGAccountBPLibrary::UXDGAccountBPLibrary(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
}

void UXDGAccountBPLibrary::Login(TArray<FString> loginTypes){
    FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("list"), loginTypes);
    Writer->WriteObjectEnd();
    Writer->Close();

    GetXDGAccountBridge()->Login(JsonOutString);
}

void UXDGAccountBPLibrary::LoginByType(FString loginType){
    GetXDGAccountBridge()->LoginByType(loginType);
}

void UXDGAccountBPLibrary::AddUserStatusChangeCallback(){
    GetXDGAccountBridge()->AddUserStatusChangeCallback();
}



void UXDGAccountBPLibrary::GetUser(){
    GetXDGAccountBridge()->GetUser();
}

void UXDGAccountBPLibrary::OpenUserCenter(){
    GetXDGAccountBridge()->OpenUserCenter();
}

void UXDGAccountBPLibrary::Logout(){
    GetXDGAccountBridge()->Logout();
    
#if PLATFORM_ANDROID || PLATFORM_IOS
    FString commandJson = TapJson::ConstructorCommand(TEXT("TapBootstrapService"),TEXT("logout"),TEXT(""),false,TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UXDGAccountBPLibrary::OpenAccountCancellation(){
    GetXDGAccountBridge()->OpenAccountCancellation();
}


bool UXDGAccountBPLibrary::IsTokenActiveWithType(FString loginType){
   bool value = GetXDGAccountBridge()->IsTokenActiveWithType(loginType);

   //测试代码
   GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("结果：" + FString::FromInt(value)));

   return value;
}

void UXDGAccountBPLibrary::BindByType(FString loginType){
    GetXDGAccountBridge()->BindByType(loginType);
}


