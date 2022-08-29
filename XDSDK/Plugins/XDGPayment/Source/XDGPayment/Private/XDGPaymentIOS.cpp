// Fill out your copyright notice in the Description page of Project Settings.

#if PLATFORM_IOS
#include "XDGPaymentIOS.h"
#include "XDGPayment.h"

XDGPaymentIOS::XDGPaymentIOS()
{
}

XDGPaymentIOS::~XDGPaymentIOS()
{
}


//ios 安卓都有的方法
void XDGPaymentIOS::QueryWithProductIds(FString listJson){
    NSArray* productIds = [XDGUE4PaymentTool getProductIdList: listJson.GetNSString()];

    [XDGPayment queryWithProductIds:productIds completionHandler:^(NSArray<XDGProductInfo *> * _Nullable result, NSError * _Nullable error) {
        NSMutableDictionary *resultDic = [NSMutableDictionary dictionary];
        __block NSMutableArray *products = [NSMutableArray array];

        [result enumerateObjectsUsingBlock:^(XDGProductInfo * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            NSDictionary *productInfo = @{
                @"localizedDescription":obj.localizedDescription?:@"",
                @"localizedTitle":obj.localizedTitle?:@"",
                @"price":obj.price,
                @"productIdentifier":obj.productIdentifier?:@"",
                @"priceLocale":@{
                        @"localeIdentifier":obj.priceLocale.localeIdentifier?:@"",
                        @"languageCode":obj.priceLocale.languageCode?:@"",
                        @"countryCode":obj.priceLocale.countryCode?:@"",
                        @"scriptCode":obj.priceLocale.scriptCode?:@"",
                        @"calendarIdentifier":obj.priceLocale.calendarIdentifier?:@"",
                        @"decimalSeparator":obj.priceLocale.decimalSeparator?:@"",
                        @"currencySymbol":obj.priceLocale.currencySymbol?:@"",
                }
            };
            [products addObject:productInfo];
        }];
        
        if (error != nil) {
            FXDGPaymentModule::OnXDGSDKQueryProductIdsFailed.Broadcast((int)error.code, UTF8_TO_TCHAR([error.localizedDescription?:@"" UTF8String]));
            NSLog(@"查询失败");
        }else{
            [resultDic setObject:products forKey:@"products"];
            NSString* json = resultDic.tdsglobal_jsonString;
            FXDGPaymentModule::OnXDGSDKQueryProductIdsSucceed.Broadcast(UTF8_TO_TCHAR([json UTF8String]));
            NSLog(@"查询成功 %@", json);
        }
    }];
}

void XDGPaymentIOS::PayWithProduct(FString orderId,
                            FString productId,
                            FString roleId,
                            FString serverId,
                            FString ext){
    dispatch_async(dispatch_get_main_queue(), ^{ 
           NSString* oid = orderId.GetNSString();
           NSString* pid = productId.GetNSString();
           NSString* rid = roleId.GetNSString();     

           //测试代码---start
            oid = @"";
            NSUserDefaults* df = [NSUserDefaults standardUserDefaults];
            rid = [df objectForKey:@"demo_tmp_userId"];
            NSLog(@"payment saved userId df get:%@", rid);
           //测试代码---end

        [XDGPayment payWithOrderId:oid productId:pid roleId:rid serverId:serverId.GetNSString() ext:ext.GetNSString() completionHandler:^(XDGOrderInfo * _Nonnull orderInfo, NSError * _Nonnull error) {
            [XDGUE4PaymentTool bridgePayCallback:orderInfo error:error];
        }];   
     });                            
}


void XDGPaymentIOS::QueryRestoredPurchases(){
    [XDGPayment queryRestoredPurchases:^(NSArray<XDGTransactionInfo *> * _Nonnull result) {
        if (result != nil){
             __block NSMutableArray *transactions = [NSMutableArray array];
            [result enumerateObjectsUsingBlock:^(XDGTransactionInfo * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                NSDictionary *transactionInfo = @{
                    @"transactionIdentifier":obj.transactionIdentifier?:@"",
                    @"productIdentifier":obj.productIdentifier?:@"",
                };
                [transactions addObject:transactionInfo];
            }];
            
            NSDictionary *resultDic = @{
                @"transactions":transactions
            };
            NSString* strJson = resultDic.tdsglobal_jsonString;
            FXDGPaymentModule::OnXDGSDKQueryRestoredPurchasesSucceed.Broadcast(UTF8_TO_TCHAR([strJson UTF8String]));
            NSLog(@"查询成功 %@", strJson);

        }else{
            FXDGPaymentModule::OnXDGSDKQueryRestoredPurchasesFailed.Broadcast(-1, TEXT("query fail"));
            NSLog(@"查询失败");
        }
    }];
}

void XDGPaymentIOS::CheckRefundStatus(){
    dispatch_async(dispatch_get_main_queue(), ^{
        [XDGPayment checkRefundStatus:^(XDGRepayMentCode code, NSString * _Nullable msg, NSDictionary * _Nullable data) {
            [XDGUE4PaymentTool bridgeRefundStatusWithCode:code msg:msg data:data];
             NSLog(@"check status %@", msg?:@"");
        }];
    });     
}


void XDGPaymentIOS::CheckRefundStatusWithUI(){
    dispatch_async(dispatch_get_main_queue(), ^{
        [XDGPayment checkRefundStatusWithUI:^(XDGRepayMentCode code, NSString * _Nullable msg, NSDictionary * _Nullable data) {
            [XDGUE4PaymentTool bridgeRefundStatusWithCode:code msg:msg data:data];
            NSLog(@"check UI %@", msg?:@"");
        }];
    });    
}


void XDGPaymentIOS::PayWithWeb(
                    FString orderId,
					FString productId, 
					FString productName,
					float  payAmount,
					FString roleId,
					FString serverId,		
					FString extras){
    //空,安卓才有网页支付

    //测试代码
    [XDGCoreService clearAllUserDefaultsData]; 
}

void XDGPaymentIOS::PurchaseToken(FString transactionIdentifier,
                            FString productIdentifier,
                            FString orderId,
                            FString roleId,
                            FString serverId,
                            FString ext){
    NSLog(@"点击 transactionIdentifier  %@", transactionIdentifier.GetNSString());

     dispatch_async(dispatch_get_main_queue(), ^{
         XDGTransactionInfo *transInfo = [[XDGTransactionInfo alloc] init];
        [transInfo setValue:transactionIdentifier.GetNSString() forKey:@"transactionIdentifier"];
        [transInfo setValue:productIdentifier.GetNSString() forKey:@"productIdentifier"];
        
        [XDGPayment restorePurchase:transInfo orderId:orderId.GetNSString() roleId:roleId.GetNSString() serverId:serverId.GetNSString() ext:ext.GetNSString() completionHandler:^(XDGOrderInfo * _Nonnull orderInfo, NSError * _Nonnull error) {
            [XDGUE4PaymentTool bridgePayCallback:orderInfo error:error];
        }];
    });                                 
   
}


//-------ios 源代码-------
@implementation XDGUE4PaymentTool

+(NSArray*)getProductIdList:(NSString*)listJson{
    if (listJson == nil || listJson.length == 0) {
        return nil;
    }else{
        NSLog(@"解析 Product List %@", listJson);
        NSData *jsonData = [listJson dataUsingEncoding:NSUTF8StringEncoding];
        NSError *err;
        NSDictionary* dic = [NSJSONSerialization JSONObjectWithData:jsonData
                                                            options:NSJSONReadingMutableContainers
                                                              error:&err];
        if (dic != nil) {
            id ary = [dic objectForKey:@"list"];
            if ([ary isKindOfClass:[NSArray class]]) {
                return ary;
            }
        }
        return nil;
    }
}


+ (void)bridgePayCallback:(XDGOrderInfo *)orderInfo error:(NSError *)error {
    if (error != nil) {
        FXDGPaymentModule::OnXDGSDKPaymentFailed.Broadcast((int)error.code, UTF8_TO_TCHAR([error.localizedDescription?:@"" UTF8String]));
        NSLog(@"失败了");
   
    }else if(orderInfo != nil){
        NSString* orderId = orderInfo.outTradeNo?:@"";
        NSString* productId = orderInfo.productIdentifier?:@"";
        NSString* serverId = orderInfo.serverId?:@"";
        NSString* roleId = orderInfo.roleId?:@"";
        FXDGPaymentModule::OnXDGSDKPaymentSucceed.Broadcast(UTF8_TO_TCHAR([orderId UTF8String]), UTF8_TO_TCHAR([productId UTF8String]), UTF8_TO_TCHAR([serverId UTF8String]), UTF8_TO_TCHAR([roleId UTF8String]));
        NSLog(@"成功了");
    }else{
        FXDGPaymentModule::OnXDGSDKPaymentFailed.Broadcast(-1, TEXT("pay fail"));
         NSLog(@"失败了2");
    }
}

+ (void)bridgeRefundStatusWithCode:(XDGRepayMentCode)code msg:(NSString *)msg data:(NSDictionary *)data{
    if (code == XDGRepayMentCodeError) {
        FXDGPaymentModule::OnXDGSDKCheckRefundStatusFailed.Broadcast(-1, UTF8_TO_TCHAR([msg?:@"" UTF8String]));
        NSLog(@"失败了");
    }else{
        NSMutableDictionary *resultDict = [NSMutableDictionary dictionary];
        resultDict[@"data"] = data?:@{};
        NSString* strJson = resultDict.tdsglobal_jsonString;
        FXDGPaymentModule::OnXDGSDKCheckRefundStatusSucceed.Broadcast(UTF8_TO_TCHAR([strJson UTF8String]));
        NSLog(@"成功了 %@", strJson);
    }
}

+ (NSString *)randomStr{
    char datas[32];
    char _randomStr[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    for (int x=0;x<32;datas[x++] =_randomStr[arc4random()%62]); //71
    return [[NSString alloc] initWithBytes:datas length:32 encoding:NSUTF8StringEncoding];
}

@end

#endif
