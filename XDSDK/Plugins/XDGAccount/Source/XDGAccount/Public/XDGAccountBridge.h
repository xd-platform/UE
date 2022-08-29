// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class XDGACCOUNT_API XDGAccountBridge
{
public:
	virtual void Login(FString loginTypeList) = 0;  //弹框登录
	virtual void LoginByType(FString loginType) = 0;  //单点登录  
	virtual void AddUserStatusChangeCallback() = 0;  //退出登录和个人中心绑定解绑回调

	virtual void GetUser() = 0; //获取本地用户信息，注意这里没网络请求
	virtual void OpenUserCenter() = 0; //打开个人中心
	virtual void Logout() = 0; //退出登录
	virtual void OpenAccountCancellation() = 0;  //打开注销页面

	virtual bool IsTokenActiveWithType(FString loginType) = 0;    
	virtual void BindByType(FString loginType) = 0;  //绑定 第三方 接口

};

XDGAccountBridge *GetXDGAccountBridge();