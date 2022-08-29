// Fill out your copyright notice in the Description page of Project Settings.


#include "XDGAccountBridge.h"


#if PLATFORM_ANDROID
#include "XDGAccountAndroid.h"

XDGAccountAndroid * GetXDGAccountAndroidBridge(){
    static XDGAccountAndroid * bridge = nullptr;
    if(bridge == nullptr){
        bridge = new XDGAccountAndroid();
    }
    return bridge;
}

XDGAccountBridge * GetXDGAccountBridge(){
    return GetXDGAccountAndroidBridge();
}

#elif PLATFORM_IOS
#include "XDGAccountIOS.h"

XDGAccountIOS * GetXDGAccountIOSBridge(){
    static XDGAccountIOS * bridge = nullptr;
    if(bridge == nullptr){
        bridge = new XDGAccountIOS();
    }
    return bridge;
}

XDGAccountBridge * GetXDGAccountBridge(){
    return GetXDGAccountIOSBridge();
}

#else

XDGAccountBridge * GetXDGAccountBridge(){
    return nullptr;
}

#endif