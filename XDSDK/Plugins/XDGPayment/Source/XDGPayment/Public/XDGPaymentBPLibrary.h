// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XDGPaymentBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class XDGPAYMENT_API UXDGPaymentBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, Category = "XDGPayment")   //iOS苹果支付 和 安卓 谷歌支付用
	static void PayWithProduct(FString orderId,
	                            FString productId,
								FString roleId,
								FString serverId,
								FString ext);

	UFUNCTION(BlueprintCallable, Category = "XDGPayment")
	static void QueryWithProductIdArray(TArray<FString> productIds);  //iOS苹果支付 和 安卓 谷歌支付用

	UFUNCTION(BlueprintCallable, Category = "XDGPayment")
	static void QueryRestoredPurchases();

	UFUNCTION(BlueprintCallable, Category = "XDGPayment")
	static void CheckRefundStatus();

	UFUNCTION(BlueprintCallable, Category = "XDGPayment")
	static void CheckRefundStatusWithUI();

	UFUNCTION(BlueprintCallable, Category = "XDGPayment")  //安卓网页支付，iOS没有
	static void PayWithWeb(
	                     	FString orderId,
							FString productId, 
							FString productName,
							float  payAmount,
							FString roleId,
							FString serverId,		
							FString extras);

	UFUNCTION(BlueprintCallable, Category = "XDGPayment")
	static void PurchaseToken(FString transactionIdentifier,
								FString productIdentifier,
								FString orderId,
								FString roleId,
								FString serverId,
								FString ext);
	
};
