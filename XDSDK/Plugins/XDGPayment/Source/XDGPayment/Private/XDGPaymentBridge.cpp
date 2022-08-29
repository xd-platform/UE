// Fill out your copyright notice in the Description page of Project Settings.


#include "XDGPaymentBridge.h"


#if PLATFORM_ANDROID
#include "XDGPaymentAndroid.h"

XDGPaymentAndroid * GetXDGPaymentAndroidBridge(){
    static XDGPaymentAndroid * bridge = nullptr;
    if(bridge == nullptr){
        bridge = new XDGPaymentAndroid();
    }
    return bridge;
}

XDGPaymentBridge * GetXDGPaymentBridge(){
    return GetXDGPaymentAndroidBridge();
}

#elif PLATFORM_IOS
#include "XDGPaymentIOS.h"

XDGPaymentIOS * GetXDGPaymentIOSBridge(){
    static XDGPaymentIOS * bridge = nullptr;
    if(bridge == nullptr){
        bridge = new XDGPaymentIOS();
    }
    return bridge;
}

XDGPaymentBridge * GetXDGPaymentBridge(){
    return GetXDGPaymentIOSBridge();
}

#else

XDGPaymentBridge * GetXDGPaymentBridge(){
    return nullptr;
}

#endif