// Fill out your copyright notice in the Description page of Project Settings.

#if PLATFORM_ANDROID
#pragma once

#include "CoreMinimal.h"
#include "XDGPaymentBridge.h"

#include "Runtime/Launch/Public/Android/AndroidJNI.h"
#include "Runtime/ApplicationCore/Public/Android/AndroidApplication.h"
#include "Runtime/Core/Public/Android/AndroidJava.h"

/**
 * 
 */
class XDGPAYMENT_API XDGPaymentAndroid : public XDGPaymentBridge
{
public:
	XDGPaymentAndroid();
	~XDGPaymentAndroid();


	void PayWithProduct(FString orderId,
						FString productId,
						FString roleId,
						FString serverId,
						FString ext);

	void QueryWithProductIds(FString listJson);

	void QueryRestoredPurchases();
	

	void CheckRefundStatus();

	void CheckRefundStatusWithUI();

	void PayWithWeb(FString orderId,
					FString productId, 
					FString productName,
					float  payAmount,
					FString roleId,
					FString serverId,		
					FString extras);

	void PurchaseToken(FString transactionIdentifier,
							FString productIdentifier,
							FString orderId,
							FString roleId,
							FString serverId,
							FString ext);				


};


#endif 