// Fill out your copyright notice in the Description page of Project Settings.

#include "XDGCommonIOS.h"
#include "Engine.h"
#include "XDGCommon.h"



XDGCommonIOS::XDGCommonIOS()
{
}

XDGCommonIOS::~XDGCommonIOS()
{
}

FString XDGCommonIOS::GetSDKVersionName(){
    FString version = "";
    NSString * nVersion = [XDGSDK getSDKVersionName];
    version = UTF8_TO_TCHAR([nVersion UTF8String]);
    NSLog(@"获取版本号 %@", nVersion);
    return version;
}

void XDGCommonIOS::SetLanguage(int32 langType){
     [XDGCoreService langType:[NSNumber numberWithInt:langType]];
     NSLog(@"设置语言 %d", langType);
}

void XDGCommonIOS::InitSDK(){
    [XDGSDK initSDK:^(BOOL success, NSString *msg) {
        if (success) {
            NSLog(@"初始化 成功： %@", msg);
        }else {
            NSLog(@"初始化 失败： %@", msg);
        }

        NSMutableDictionary *tapConfig = [NSMutableDictionary dictionary];
        TapConfig *config = [XDConfigManager currentConfig].tapConfig;
        [tapConfig setValue:@(config.region) forKey:@"region"];
        [tapConfig setValue:config.clientId forKey:@"clientId"];
        [tapConfig setValue:config.clientToken forKey:@"clientToken"];
        [tapConfig setValue:config.serverURL forKey:@"serverURL"];
        
        NSMutableDictionary *dbConfig = [NSMutableDictionary dictionary];
        [dbConfig setValue:@(config.dbConfig.enable) forKey:@"enableTapDB"];
        if (config.dbConfig.enable) {
            [dbConfig setValue:config.dbConfig.channel forKey:@"channel"];
            [dbConfig setValue:config.dbConfig.gameVersion forKey:@"gameVersion"];
            [dbConfig setValue:@(config.dbConfig.advertiserIDCollectionEnabled) forKey:@"idfa"];
        }
        [tapConfig setValue:dbConfig forKey:@"dbConfig"];
        
        NSDictionary *resultDic = @{
            @"success":@(success),
            @"message":msg,
            @"configInfo":tapConfig
        };
        
        NSString* result = resultDic.tdsglobal_jsonString;
        FXDGCommonModule::OnXDGSDKInitCompleted.Broadcast((bool)success, UTF8_TO_TCHAR([result UTF8String]));
    }];
}

bool XDGCommonIOS::IsInitialized(){
    bool isLoggedIn = false;
    isLoggedIn = [XDConfigManager isGameInited];
    NSLog(@"是否初始化了 %d", isLoggedIn);
    return isLoggedIn;
}

void XDGCommonIOS::Report(FString serverId, FString roleId, FString roleName){
    [XDGSDK reportWithServerId:serverId.GetNSString() roleId:roleId.GetNSString() roleName:roleName.GetNSString()];
    NSLog(@"点击了 Report %@", serverId.GetNSString());
}

void XDGCommonIOS::StoreReview(){
    [XDGSDK storeReview];
    NSLog(@"点击 StoreReview");
}

void XDGCommonIOS::ShareFlavors(int32 type, FString uri, FString message){
    // NSLog(@"点击 ShareFlavors %@", uri.GetNSString());

    // dispatch_async(dispatch_get_main_queue(), ^{
    //     XDGShareType shareType = XDGShareTypeFacebook;//0
    //     if(type == 1){
    //         shareType = XDGShareTypeLine; //1
    //     }else if(type == 2){
    //         shareType = XDGShareTypeTwitter; //2
    //     }

    //     [XDGShare shareWithType:shareType url:uri.GetNSString() message:message.GetNSString() completeHandler:^(NSError * _Nullable error, BOOL cancel) {
    //         [XDGUE4CommonTool shareWithResult:error cancel:cancel];
    //     }];   
    // });
}

void XDGCommonIOS::ShareImage(int32 type, FString imagePath){
    // dispatch_async(dispatch_get_main_queue(), ^{
    //     XDGShareType shareType = XDGShareTypeFacebook;//0
    //     if(type == 1){
    //         shareType = XDGShareTypeLine; //1
    //     }else if(type == 2){
    //         shareType = XDGShareTypeTwitter; //2
    //     }

    //     UIImage *image = nil;
    //     NSString* str = imagePath.GetNSString();
    //     if ([str hasPrefix:@"http"]) { //网络图片
    //         NSData *imageData = [NSData dataWithContentsOfURL:[NSURL URLWithString:str]]; 
    //         image = [[UIImage alloc] initWithData:imageData];

    //     }else{ //沙盒路径图片
    //         image = [UIImage imageWithContentsOfFile:str];
    //     }


    //     // 沙盒测试路径，代码测试用
    //     // NSString* path = [[NSBundle mainBundle] pathForResource:@"XDGResources" ofType:@"bundle"];
    //     // NSString* str = [path stringByAppendingString:@"/tds_apple_icon@3x.png"];

    //     if (!image || image == nil) {
    //         NSError *error = [NSError errorWithDomain:@"com.tdsglobal.share" code:-1 userInfo:@{NSLocalizedDescriptionKey:[NSString stringWithFormat: @"can not find image with path:%@",imagePath.GetNSString()]}];
    //         [XDGUE4CommonTool shareWithResult:error cancel:NO];
    //         NSLog(@"分享图片为空");
    //     }
    //     [XDGShare shareWithType:shareType image:image completeHandler:^(NSError * _Nullable error, BOOL cancel) {
    //         [XDGUE4CommonTool shareWithResult:error cancel:cancel];
    //     }];
    // });
}

void XDGCommonIOS::TrackUser(FString userId){
    [XDGTrackerManager trackUser:userId.GetNSString()];
    NSLog(@"点击 TrackUser %@", userId.GetNSString());
}

void XDGCommonIOS::TrackRole(FString serverId, FString roleId, FString roleName, int32 level){
    [XDGTrackerManager trackRoleWithRoleId:roleId.GetNSString() roleName:roleName.GetNSString() serverId:serverId.GetNSString() level:(NSInteger)level];
    NSLog(@"点击 TrackRole %@", roleId.GetNSString());
}

void XDGCommonIOS::TrackEvent(FString eventName){
    [XDGTrackerManager trackEvent:eventName.GetNSString()];
    NSLog(@"点击 TrackEvent %@", eventName.GetNSString());
}


void XDGCommonIOS::SetCurrentUserPushServiceEnable(bool enable){
    dispatch_async(dispatch_get_main_queue(), ^{
        [XDGSDK setCurrentUserPushServiceEnable:enable];
         NSLog(@"点击 PushServiceEnable %d", enable);
    });
}

bool XDGCommonIOS::IsCurrentUserPushServiceEnable(){
    bool isEnable = false;
    isEnable = [XDGSDK isCurrentUserPushServiceEnable];
    NSLog(@"是否开通通知了 %d", isEnable);
    return isEnable;
}


void XDGCommonIOS::GetRegionInfo(){
     dispatch_async(dispatch_get_main_queue(), ^{
        [XDGSDK getRegionInfo:^(XDGRegionInfo * _Nonnull info) {
            NSString* countryCode = info.countryCode;
            NSString* city = info.city;
            NSString* timeZone = info.timeZone;
            NSString* locationInfoType = info.locationInfoType;
        
            NSLog(@"点击 GetRegionInfo %@  -  %@  - %@ - %@", countryCode, city, timeZone, locationInfoType);
            FXDGCommonModule::OnXDGSDKGetRegionInfoCompleted.Broadcast(UTF8_TO_TCHAR([countryCode?:@"" UTF8String]), UTF8_TO_TCHAR([city?:@"" UTF8String]), UTF8_TO_TCHAR([timeZone?:@"" UTF8String]), UTF8_TO_TCHAR([locationInfoType?:@"" UTF8String]));
        }];
    });
}


void XDGCommonIOS::GetXDGInfoJson(){
  //iOS不需要
}


void XDGCommonIOS::DisableAgreementUI(){
    [XDGSDKSettings setAgreementUIEnable:NO];
}

void XDGCommonIOS::SetCountryRegion(FString region){
    [XDGSDKSettings setTargetCountryOrRegion:region.GetNSString()];
}

void XDGCommonIOS::DevelopInit(int32 num){
    // 0: 海外 正式	1：海外 RND	2：国内 正式	3.国内 RND
    if (num == 0) {
        [XDGSDKSettings updateConfigFileName:@"io_config_pro"];
    } else if (num == 1) {
        [XDGSDKSettings updateConfigFileName:@"io_config_test"];
    } else if (num == 2) {
        [XDGSDKSettings updateConfigFileName:@"cn_config_pro"];
    } else if (num == 3) {
        [XDGSDKSettings updateConfigFileName:@"cn_config_test"];
    }

    if (num == 0 || num == 2) {
        [[TDSHostReplaceUtil shareInstance] addReplacedHostPair:@"https://xdsdk-6.xd.cn" replacedHost:@"https://xdsdk-6.xd.cn"];
        [[TDSHostReplaceUtil shareInstance] addReplacedHostPair:@"https://xdsdk-intnl-6.xd.com" replacedHost:@"https://xdsdk-intnl-6.xd.com"];
        [[TDSHostReplaceUtil shareInstance] addReplacedHostPair:@"https://ecdn-xdsdk-intnl-6.xd.com" replacedHost:@"https://ecdn-xdsdk-intnl-6.xd.com"];
        [[TDSHostReplaceUtil shareInstance] addReplacedHostPair: @"https://event-tracking-cn.cn-beijing.log.aliyuncs.com/logstores/sdk6-prod/track" replacedHost:@"https://event-tracking-cn.cn-beijing.log.aliyuncs.com/logstores/sdk6-prod/track"];
        [[TDSHostReplaceUtil shareInstance] addReplacedHostPair:@"https://event-tracking-global.ap-southeast-1.log.aliyuncs.com/logstores/sdk6-prod/track" replacedHost:@"https://event-tracking-global.ap-southeast-1.log.aliyuncs.com/logstores/sdk6-prod/track"];
    } else if (num == 1 || num == 3) {
        [[TDSHostReplaceUtil shareInstance] addReplacedHostPair:@"https://xdsdk-6.xd.cn" replacedHost:@"https://test-xdsdk-6.xd.cn"];
        [[TDSHostReplaceUtil shareInstance] addReplacedHostPair:@"https://xdsdk-intnl-6.xd.com" replacedHost:@"https://test-xdsdk-intnl-6.xd.com"];
        [[TDSHostReplaceUtil shareInstance] addReplacedHostPair:@"https://ecdn-xdsdk-intnl-6.xd.com" replacedHost:@"https://test-xdsdk-intnl-6.xd.com"];
        [[TDSHostReplaceUtil shareInstance] addReplacedHostPair: @"https://event-tracking-cn.cn-beijing.log.aliyuncs.com/logstores/sdk6-prod/track" replacedHost:@"https://event-tracking-cn.cn-beijing.log.aliyuncs.com/logstores/sdk6-test/track"];
        [[TDSHostReplaceUtil shareInstance] addReplacedHostPair:@"https://event-tracking-global.ap-southeast-1.log.aliyuncs.com/logstores/sdk6-prod/track" replacedHost:@"https://event-tracking-global.ap-southeast-1.log.aliyuncs.com/logstores/sdk6-test/track"];
    }
    InitSDK();
}

void XDGCommonIOS::ClearAllUserDefaultsData() {
    NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];

    NSDictionary *dic =[userDefaults dictionaryRepresentation];

    for (id key in dic) {
        [userDefaults removeObjectForKey:key];
    }

    [userDefaults synchronize];
}

void XDGCommonIOS::TrackAchievement(){
    [XDGTrackerManager trackAchievement];
     NSLog(@"点击 TrackAchievement");
}

void XDGCommonIOS::EventCompletedTutorial(){
    [XDGTrackerManager eventCompletedTutorial];
     NSLog(@"点击 EventCompletedTutorial");
}

void XDGCommonIOS::EventCreateRole(){
    [XDGTrackerManager eventCreateRole];
     NSLog(@"点击 EventCreateRole");
}


//-------ios 源代码-------
@implementation XDGUE4CommonTool

 //0成功, 1取消, 2失败
+ (void)shareWithResult:(NSError *)error cancel:(BOOL)cancel {
    if(error != nil){
        FXDGCommonModule::OnXDGSDKShareCompleted.Broadcast(2);
    }else if(cancel){
        FXDGCommonModule::OnXDGSDKShareCompleted.Broadcast(1);
    }else{
        FXDGCommonModule::OnXDGSDKShareCompleted.Broadcast(0);
    }
}

@end
