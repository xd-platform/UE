// Fill out your copyright notice in the Description page of Project Settings.

#if PLATFORM_IOS
#pragma once

#include "CoreMinimal.h"
#include "XDGPaymentBridge.h"

#import <Foundation/Foundation.h>
#import <XDPaymentSDK/XDGOrderInfo.h>
#import <XDPaymentSDK/XDGTransactionInfo.h>
#import <XDPaymentSDK/XDGPayment.h>
#import <XDPaymentSDK/XDGProductInfo.h>
#import <XDCommonSDK/NSDictionary+TDSGlobalJson.h>
#import <XDCommonSDK/XDGCoreService.h>


/**
 * 
 */
class XDGPAYMENT_API XDGPaymentIOS : public XDGPaymentBridge
{
public:
	XDGPaymentIOS();
	~XDGPaymentIOS();

	void PayWithProduct(FString orderId,
						FString productId,
						FString roleId,
						FString serverId,
						FString ext);

	void QueryWithProductIds(FString listJson);

	void QueryRestoredPurchases();

	

	void CheckRefundStatus();

	void CheckRefundStatusWithUI();

	void PayWithWeb(
					FString orderId,
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


@interface XDGUE4PaymentTool : NSObject

+(NSArray*)getProductIdList:(NSString*)listJson;

+ (void)bridgePayCallback:(XDGOrderInfo *)orderInfo error:(NSError *)error;

+ (void)bridgeRefundStatusWithCode:(XDGRepayMentCode)code msg:(NSString *)msg data:(NSDictionary *)data;

+ (NSString *)randomStr;

@end


#endif