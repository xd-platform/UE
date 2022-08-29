// Fill out your copyright notice in the Description page of Project Settings.

#if PLATFORM_IOS
#include "XDGAccountIOS.h"

#include "Engine.h"
#include "XDGAccount.h"


XDGAccountIOS::XDGAccountIOS()
{
}

XDGAccountIOS::~XDGAccountIOS()
{
}

void XDGAccountIOS::Login(FString loginTypeList){
   [XDGUE4AccountTool enableTap:^{
        dispatch_async(dispatch_get_main_queue(), ^{
           
            NSArray* ary = [XDGUE4AccountTool getLoginTypeList: loginTypeList.GetNSString()];


            if(ary == nil){
                NSLog(@"登录参数是空：%@",loginTypeList.GetNSString());
            }else{
                NSLog(@"登录参数：%@", loginTypeList.GetNSString());

                XDGLoginEntriesConfig *config = [XDGLoginEntriesConfig new];
                for (NSString* item in ary) {
                    if(item != nil && item.length > 0){
                        [config.loginEntryList addObject:[item uppercaseString]];
                    }
                }
                
                [XDGAccount loginWithConfig:config handler:^(XDGUser * _Nullable result, NSError * _Nullable error) {
                    [XDGUE4AccountTool bridgeUserCallback:result error:error isLogin: YES];
                }];
            }
        });
    }];
}

void XDGAccountIOS::LoginByType(FString loginType){
    [XDGUE4AccountTool enableTap:^{
        dispatch_async(dispatch_get_main_queue(), ^{
          NSLog(@"单点登录类型：%@", loginType.GetNSString());

          LoginEntryType type = [XDGEntryType entryTypeByString:loginType.GetNSString()];
          [XDGAccount loginByType:type loginHandler:^(XDGUser * _Nullable result, NSError * _Nullable error) {
              [XDGUE4AccountTool bridgeUserCallback:result error:error isLogin: YES];
          }]; 
       });
    }];
}

void XDGAccountIOS::AddUserStatusChangeCallback(){
    [XDGAccount addUserStatusChangeCallback:^(XDGUserStateChangeCode userStateChangeCode,NSString *message) {
        NSLog(@"添加回调 %d  %@", (int)userStateChangeCode, message);
        FXDGAccountModule::OnXDGSDKUserStateChanged.Broadcast((int)userStateChangeCode, UTF8_TO_TCHAR([message?:@"" UTF8String]));
    }];
}

void XDGAccountIOS::GetUser(){
    [XDGAccount getUser:^(XDGUser * _Nullable result, NSError * _Nullable error) {
        [XDGUE4AccountTool bridgeUserCallback:result error:error isLogin: NO];
    }];
}

void XDGAccountIOS::OpenUserCenter(){
     dispatch_async(dispatch_get_main_queue(), ^{
        [XDGAccount openUserCenter];
         NSLog(@"打开用户中心");
    });
}

void XDGAccountIOS::Logout(){
    dispatch_async(dispatch_get_main_queue(), ^{
        [XDGAccount logout];
        NSLog(@"点击退出登录");
    });
}

//打开注销页面
void XDGAccountIOS::OpenAccountCancellation(){
     dispatch_async(dispatch_get_main_queue(), ^{
        [XDGAccount accountCancellation];
        NSLog(@"打开注销页面");
    });
}

bool XDGAccountIOS::IsTokenActiveWithType(FString loginType){
    LoginEntryType type = [XDGEntryType entryTypeByString:loginType.GetNSString()];
    BOOL isTokenActive = [XDGThirdTokenManager tokenActiveWithType:type];
    return isTokenActive;
}

void XDGAccountIOS::BindByType(FString loginType){
     LoginEntryType type = [XDGEntryType entryTypeByString:loginType.GetNSString()];

     [XDGAccount bindByType:type bindHandler:^(BOOL success, NSError *_Nullable error) {
        int code = 1;
        NSString* msg = @"success";
        if (!success && error != nil){
            code = error.code;
            msg = error.localizedDescription?:@"";
        }

        FXDGAccountModule::OnXDGSDKBindByTypeCompleted.Broadcast(success, (int)code, UTF8_TO_TCHAR([msg UTF8String]));
     }];
}


//-------ios 源代码-------
@implementation XDGUE4AccountTool
NSTimeInterval lastClick;        //防止重复点击
NSTimeInterval gapTime = 1000;   //点击间隔毫秒 

+(NSArray*)getLoginTypeList:(NSString*)listJson{
    if (listJson == nil || listJson.length == 0) {
        return nil;
    }else{
        NSLog(@"解析 Login List %@", listJson);
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


+ (void)enableTap:(VoidTapCallback) block{
    NSTimeInterval time = [[NSDate date] timeIntervalSince1970] * 1000;
    if ((time - lastClick) > gapTime) {
        lastClick = time;
        block();
    }
}

+ (void)bridgeUserCallback:(XDGUser *)user error:(NSError *)error isLogin: (Boolean)isLogin{
    if (error != nil) {
        if(isLogin){
            NSDictionary* data = error.userInfo[@"extraData"]?:@{};
            NSString* dataJson = data.tdsglobal_jsonString;

            FXDGAccountModule::OnXDGSDKLoginFailed.Broadcast((int)error.code, UTF8_TO_TCHAR([error.localizedDescription?:@"" UTF8String]), UTF8_TO_TCHAR([dataJson UTF8String]));
        }else{
            FXDGAccountModule::OnXDGSDKGetUserFailed.Broadcast((int)error.code, UTF8_TO_TCHAR([error.localizedDescription?:@"" UTF8String]));
        }   
        NSLog(@"失败：%@", error.localizedDescription?:@"");

    }else if(user != nil){
         NSDictionary *userDic = @{
            @"userId":user.userId,
            @"name":user.name?:@"",
            @"loginType":user.loginType,
            @"boundAccounts":user.boundAccounts?:@[],
            @"token":@{
                    @"kid":user.token.kid?:@"",
                    @"tokenType":user.token.tokenType?:@"",
                    @"macKey":user.token.macKey?:@"",
                    @"macAlgorithm":user.token.macAlgorithm?:@"",
            }
        };
        NSString* res = userDic.tdsglobal_jsonString;

        if(isLogin){
            FXDGAccountModule::OnXDGSDKLoginSucceed.Broadcast(UTF8_TO_TCHAR([res UTF8String]));
        }else{
            FXDGAccountModule::OnXDGSDKGetUserSucceed.Broadcast(UTF8_TO_TCHAR([res UTF8String]));
        }   
        NSLog(@"成功：%@", res);

        //测试代码--start
            NSString* uid = user.userId;
            NSUserDefaults* df = [NSUserDefaults standardUserDefaults];
            [df setValue:uid forKey:@"demo_tmp_userId"];
            [df synchronize];
            NSLog(@"saved userId df get:%@", [df objectForKey:@"demo_tmp_userId"]);
        //测试代码--end

    }else{
        if(isLogin){
            FXDGAccountModule::OnXDGSDKLoginFailed.Broadcast(-1, TEXT("fail"), "");
        }else{
            FXDGAccountModule::OnXDGSDKGetUserFailed.Broadcast(-1, TEXT("fail"));
        }   
        
        NSLog(@"失败2");
    }
}

@end

#endif 
