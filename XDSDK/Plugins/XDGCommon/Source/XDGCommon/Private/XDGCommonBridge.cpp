// Fill out your copyright notice in the Description page of Project Settings.


#include "XDGCommonBridge.h"

#if PLATFORM_ANDROID
#include "XDGCommonAndroid.h"

XDGCommonAndroid * GetXDGCommonAndroidBridge(){
    static XDGCommonAndroid * bridge = nullptr;
    if(bridge == nullptr){
        bridge = new XDGCommonAndroid();
    }
    return bridge;
}

XDGCommonBridge * GetXDGCommonBridge(){
    return GetXDGCommonAndroidBridge();
}

#elif PLATFORM_IOS
#include "XDGCommonIOS.h"

XDGCommonIOS * GetXDGCommonIOSBridge(){
    static XDGCommonIOS * bridge = nullptr;
    if(bridge == nullptr){
        bridge = new XDGCommonIOS();
    }
    return bridge;
}

XDGCommonBridge * GetXDGCommonBridge(){
    return GetXDGCommonIOSBridge();
}

#else

XDGCommonBridge * GetXDGCommonBridge(){
    return nullptr;
}

#endif