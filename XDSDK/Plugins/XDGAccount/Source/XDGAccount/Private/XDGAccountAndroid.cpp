// Fill out your copyright notice in the Description page of Project Settings.

#if PLATFORM_ANDROID
#include "XDGAccountAndroid.h"

#include "Engine.h"
#include "XDGAccount.h"

#define UNREAL4_CLASS_NAME_ACCOUNT "com/xd/XDGAccountUnreal4"

XDGAccountAndroid::XDGAccountAndroid(){

}

XDGAccountAndroid::~XDGAccountAndroid(){

}

void XDGAccountAndroid::Login(FString loginTypeList){
    JNIEnv *env = FAndroidApplication::GetJavaEnv();
    auto jXDSDKUnreal4Class = FAndroidApplication::FindJavaClass(UNREAL4_CLASS_NAME_ACCOUNT);
    if (jXDSDKUnreal4Class)
    {
        const char *strMethod = "login";
        auto jMethod = env->GetStaticMethodID(jXDSDKUnreal4Class, strMethod,
                                              "(Ljava/lang/String;)V");
        if (jMethod)
        {
            auto jLists = env->NewStringUTF(TCHAR_TO_UTF8(*loginTypeList));

            env->CallStaticVoidMethod(jXDSDKUnreal4Class, jMethod, jLists);

            env->DeleteLocalRef(jLists);
        }
    }
    env->DeleteLocalRef(jXDSDKUnreal4Class);   
}

void XDGAccountAndroid::LoginByType(FString loginType){
    JNIEnv *env = FAndroidApplication::GetJavaEnv();
    auto jXDSDKUnreal4Class = FAndroidApplication::FindJavaClass(UNREAL4_CLASS_NAME_ACCOUNT);
    if (jXDSDKUnreal4Class)
    {
        const char *strMethod = "loginByType";
        auto jMethod = env->GetStaticMethodID(jXDSDKUnreal4Class, strMethod,
                                              "(Ljava/lang/String;)V");
        if (jMethod)
        {
            auto jLoginType = env->NewStringUTF(TCHAR_TO_UTF8(*loginType));

            env->CallStaticVoidMethod(jXDSDKUnreal4Class, jMethod, jLoginType);
            env->DeleteLocalRef(jLoginType);
        }
    }
    env->DeleteLocalRef(jXDSDKUnreal4Class);
}

void XDGAccountAndroid::AddUserStatusChangeCallback(){
    JNIEnv *env = FAndroidApplication::GetJavaEnv();
    auto jXDSDKUnreal4Class = FAndroidApplication::FindJavaClass(UNREAL4_CLASS_NAME_ACCOUNT);
    if (jXDSDKUnreal4Class)
    {
        const char *strMethod = "addUserStatusChangeCallback";
        auto jMethod = env->GetStaticMethodID(jXDSDKUnreal4Class, strMethod,
                                              "()V");
        if (jMethod)
        {
            env->CallStaticVoidMethod(jXDSDKUnreal4Class, jMethod);
        }
    }
    env->DeleteLocalRef(jXDSDKUnreal4Class); 
}

void XDGAccountAndroid::GetUser(){
    JNIEnv *env = FAndroidApplication::GetJavaEnv();
    auto jXDSDKUnreal4Class = FAndroidApplication::FindJavaClass(UNREAL4_CLASS_NAME_ACCOUNT);
    if (jXDSDKUnreal4Class)
    {
        const char *strMethod = "getUser";
        auto jMethod = env->GetStaticMethodID(jXDSDKUnreal4Class, strMethod,
                                              "()V");
        if (jMethod)
        {
            env->CallStaticVoidMethod(jXDSDKUnreal4Class, jMethod);
        }
    }
    env->DeleteLocalRef(jXDSDKUnreal4Class); 
}

void XDGAccountAndroid::OpenUserCenter(){
    JNIEnv *env = FAndroidApplication::GetJavaEnv();
    auto jXDSDKUnreal4Class = FAndroidApplication::FindJavaClass(UNREAL4_CLASS_NAME_ACCOUNT);
    if (jXDSDKUnreal4Class)
    {
        const char *strMethod = "openUserCenter";
        auto jMethod = env->GetStaticMethodID(jXDSDKUnreal4Class, strMethod,
                                              "()V");
        if (jMethod)
        {
            env->CallStaticVoidMethod(jXDSDKUnreal4Class, jMethod);
        }
    }
    env->DeleteLocalRef(jXDSDKUnreal4Class); 
}

void XDGAccountAndroid::Logout(){
    JNIEnv *env = FAndroidApplication::GetJavaEnv();
    auto jXDSDKUnreal4Class = FAndroidApplication::FindJavaClass(UNREAL4_CLASS_NAME_ACCOUNT);
    if (jXDSDKUnreal4Class)
    {
        const char *strMethod = "logout";
        auto jMethod = env->GetStaticMethodID(jXDSDKUnreal4Class, strMethod,
                                              "()V");
        if (jMethod)
        {
            env->CallStaticVoidMethod(jXDSDKUnreal4Class, jMethod);
        }
    }
    env->DeleteLocalRef(jXDSDKUnreal4Class); 
}


void XDGAccountAndroid::OpenAccountCancellation(){
    JNIEnv *env = FAndroidApplication::GetJavaEnv();
    auto jXDSDKUnreal4Class = FAndroidApplication::FindJavaClass(UNREAL4_CLASS_NAME_ACCOUNT);
    if (jXDSDKUnreal4Class)
    {
        const char *strMethod = "accountCancellation";
        auto jMethod = env->GetStaticMethodID(jXDSDKUnreal4Class, strMethod,
                                              "()V");
        if (jMethod)
        {
            env->CallStaticVoidMethod(jXDSDKUnreal4Class, jMethod);
        }
    }
    env->DeleteLocalRef(jXDSDKUnreal4Class); 
}


bool XDGAccountAndroid::IsTokenActiveWithType(FString loginType){
    bool isActive = false;
    JNIEnv *env = FAndroidApplication::GetJavaEnv();
    auto jXDSDKUnreal4Class = FAndroidApplication::FindJavaClass(UNREAL4_CLASS_NAME_ACCOUNT);
    if (jXDSDKUnreal4Class)
    {
        const char *strMethod = "isTokenActiveWithType";
        auto jMethod = env->GetStaticMethodID(jXDSDKUnreal4Class, strMethod,
                                              "(Ljava/lang/String;)Z");
        if (jMethod)
        {
            auto jLoginType = env->NewStringUTF(TCHAR_TO_UTF8(*loginType));

            isActive = (bool)(env->CallStaticBooleanMethod(jXDSDKUnreal4Class, jMethod, jLoginType));

            env->DeleteLocalRef(jLoginType);
        }
    }
    env->DeleteLocalRef(jXDSDKUnreal4Class);
    return isActive;
}


void XDGAccountAndroid::BindByType(FString loginType){
    JNIEnv *env = FAndroidApplication::GetJavaEnv();
    auto jXDSDKUnreal4Class = FAndroidApplication::FindJavaClass(UNREAL4_CLASS_NAME_ACCOUNT);
    if (jXDSDKUnreal4Class)
    {
        const char *strMethod = "bindByType";
        auto jMethod = env->GetStaticMethodID(jXDSDKUnreal4Class, strMethod,
                                              "(Ljava/lang/String;)V");
        if (jMethod)
        {
            auto jLoginType = env->NewStringUTF(TCHAR_TO_UTF8(*loginType));

            env->CallStaticVoidMethod(jXDSDKUnreal4Class, jMethod, jLoginType);

            env->DeleteLocalRef(jLoginType);
        }
    }
    env->DeleteLocalRef(jXDSDKUnreal4Class);
}


#ifdef __cplusplus
extern "C"
{
#endif

  __attribute__((visibility("default"))) void Java_com_xd_XDGAccountUnreal4_nativeOnXDGSDKBindByTypeCompleted(JNIEnv *jenv, jclass thiz, jboolean success ,int32 code ,jstring msg)
    {
        const char *cMsg = jenv->GetStringUTFChars(msg, 0);
        FString fMsg = UTF8_TO_TCHAR(cMsg);
        FXDGAccountModule::OnXDGSDKBindByTypeCompleted.Broadcast((bool)success, (int)code, fMsg);
        jenv->ReleaseStringUTFChars(msg, cMsg);
    }

    __attribute__((visibility("default"))) void Java_com_xd_XDGAccountUnreal4_nativeOnXDGSDKLoginSucceed(JNIEnv *jenv, jclass thiz, jstring userJson)
    {
        const char *cJson = jenv->GetStringUTFChars(userJson, 0);
        FString fJson = UTF8_TO_TCHAR(cJson);
        FXDGAccountModule::OnXDGSDKLoginSucceed.Broadcast(fJson);
        jenv->ReleaseStringUTFChars(userJson, cJson);
    }

     __attribute__((visibility("default"))) void Java_com_xd_XDGAccountUnreal4_nativeOnXDGSDKLoginFailed(JNIEnv *jenv, jclass thiz, int32 code ,jstring msg, jstring data)
    {
        const char *cMsg = jenv->GetStringUTFChars(msg, 0);
        FString fMsg = UTF8_TO_TCHAR(cMsg);

        const char *cData = jenv->GetStringUTFChars(data, 0);
        FString fData = UTF8_TO_TCHAR(cData);

        FXDGAccountModule::OnXDGSDKLoginFailed.Broadcast((int)code, fMsg, fData);

        jenv->ReleaseStringUTFChars(msg, cMsg);
        jenv->ReleaseStringUTFChars(data, cData);
    }

     __attribute__((visibility("default"))) void Java_com_xd_XDGAccountUnreal4_nativeOnXDGSDKGetUserSucceed(JNIEnv *jenv, jclass thiz, jstring userJson)
    {
        const char *cJson = jenv->GetStringUTFChars(userJson, 0);
        FString fJson = UTF8_TO_TCHAR(cJson);
        FXDGAccountModule::OnXDGSDKGetUserSucceed.Broadcast(fJson);
        jenv->ReleaseStringUTFChars(userJson, cJson);
    }

     __attribute__((visibility("default"))) void Java_com_xd_XDGAccountUnreal4_nativeOnXDGSDKGetUserFailed(JNIEnv *jenv, jclass thiz, int32 code ,jstring msg)
    {
        const char *cMsg = jenv->GetStringUTFChars(msg, 0);
        FString fMsg = UTF8_TO_TCHAR(cMsg);
        FXDGAccountModule::OnXDGSDKGetUserFailed.Broadcast((int)code, fMsg);
        jenv->ReleaseStringUTFChars(msg, cMsg);
    }

      __attribute__((visibility("default"))) void Java_com_xd_XDGAccountUnreal4_nativeOnXDGSDKUserStateChanged(JNIEnv *jenv, jclass thiz, int32 code ,jstring msg)
    {
        const char *cMsg = jenv->GetStringUTFChars(msg, 0);
        FString fMsg = UTF8_TO_TCHAR(cMsg);
        FXDGAccountModule::OnXDGSDKUserStateChanged.Broadcast((int)code, fMsg);
        jenv->ReleaseStringUTFChars(msg, cMsg);
    }

#ifdef __cplusplus
}
#endif

#endif