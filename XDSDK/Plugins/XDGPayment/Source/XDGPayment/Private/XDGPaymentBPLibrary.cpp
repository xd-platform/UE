// Fill out your copyright notice in the Description page of Project Settings.


#include "XDGPaymentBPLibrary.h"
#include "XDGPaymentBridge.h"
#include "Engine.h"
#include "TapJson.h"

UXDGPaymentBPLibrary::UXDGPaymentBPLibrary(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
}

//ios 安卓都有的方法
void UXDGPaymentBPLibrary::QueryWithProductIdArray(TArray<FString> productIds){
    FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("list"), productIds);
    Writer->WriteObjectEnd();
    Writer->Close();

    GetXDGPaymentBridge()->QueryWithProductIds(JsonOutString);    
}

void UXDGPaymentBPLibrary::PayWithProduct(FString orderId,
                                        FString productId,
                                        FString roleId,
                                        FString serverId,
                                        FString ext){
    GetXDGPaymentBridge()->PayWithProduct(orderId, productId, roleId, serverId, ext);
}


void UXDGPaymentBPLibrary::QueryRestoredPurchases(){
    GetXDGPaymentBridge()->QueryRestoredPurchases();
}


void UXDGPaymentBPLibrary::CheckRefundStatus(){
    GetXDGPaymentBridge()->CheckRefundStatus();
}


void UXDGPaymentBPLibrary::CheckRefundStatusWithUI(){
    GetXDGPaymentBridge()->CheckRefundStatusWithUI();
}

void UXDGPaymentBPLibrary::PayWithWeb(
                            FString orderId,
							FString productId, 
							FString productName,
							float  payAmount,
							FString roleId,
							FString serverId,		
							FString extras){
    GetXDGPaymentBridge()->PayWithWeb(orderId, productId, productName, payAmount, roleId, serverId, extras);                                       
}


void UXDGPaymentBPLibrary::PurchaseToken(FString transactionIdentifier,
                                        FString productIdentifier,
                                        FString orderId,
                                        FString roleId,
                                        FString serverId,
                                        FString ext){
    GetXDGPaymentBridge()->PurchaseToken(transactionIdentifier, productIdentifier, orderId, roleId, serverId, ext);
}
