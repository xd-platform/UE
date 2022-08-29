// Fill out your copyright notice in the Description page of Project Settings.

#if PLATFORM_ANDROID
#pragma once

#include "Core.h"
#include "XDGAccountBridge.h"

#include "Runtime/Launch/Public/Android/AndroidJNI.h"
#include "Runtime/ApplicationCore/Public/Android/AndroidApplication.h"
#include "Runtime/Core/Public/Android/AndroidJava.h"

/**
 * 
 */
class XDGACCOUNT_API XDGAccountAndroid : public XDGAccountBridge
{
public:
	XDGAccountAndroid();
	~XDGAccountAndroid();

	void Login(FString loginTypeList);
	void LoginByType(FString loginType);
	void AddUserStatusChangeCallback();

    void GetUser();
	void OpenUserCenter();
	void Logout();
	void OpenAccountCancellation();

	bool IsTokenActiveWithType(FString loginType);    
	void BindByType(FString loginType); 

};

#endif 