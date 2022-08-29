// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class XDGPAYMENT_API XDGPaymentBridge
{
public:

	virtual void PayWithProduct(FString orderId,
	                            FString productId,
								FString roleId,
								FString serverId,
								FString ext) = 0;

	virtual void QueryWithProductIds(FString listJson) = 0;

	virtual void QueryRestoredPurchases() = 0;

	virtual void CheckRefundStatus() = 0;

	virtual void CheckRefundStatusWithUI() = 0;

	virtual void PayWithWeb(FString orderId,
							FString productId, 
							FString productName,
							float  payAmount,
							FString roleId,
							FString serverId,		
							FString extras) = 0;
    
	virtual void PurchaseToken(FString transactionIdentifier,
								FString productIdentifier,
								FString orderId,
								FString roleId,
								FString serverId,
								FString ext) = 0;

};

XDGPaymentBridge *GetXDGPaymentBridge();