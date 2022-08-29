// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XDGAccountBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class XDGACCOUNT_API UXDGAccountBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, Category = "XDGAccount")
	static void Login(TArray<FString> loginTypes);  //弹框登录，参数：弹框里的登录类型

	UFUNCTION(BlueprintCallable, Category = "XDGAccount")
	static void LoginByType(FString loginType);  //单点登录

	UFUNCTION(BlueprintCallable, Category = "XDGAccount")
	static void AddUserStatusChangeCallback();  //绑定回调(退出和个人中心里的绑定解绑回调)



    UFUNCTION(BlueprintCallable, Category = "XDGAccount")
	static void GetUser(); //获取本地用户信息

	UFUNCTION(BlueprintCallable, Category = "XDGAccount")
	static void OpenUserCenter(); //打开个人中心

	UFUNCTION(BlueprintCallable, Category = "XDGAccount")
	static void Logout();   //退出登录

	UFUNCTION(BlueprintCallable, Category = "XDGAccount")
	static void OpenAccountCancellation();  //打开账号注销页面



	UFUNCTION(BlueprintCallable, Category = "XDGAccount")
	static bool IsTokenActiveWithType(FString loginType);   //token是否过期

	UFUNCTION(BlueprintCallable, Category = "XDGAccount")
	static void BindByType(FString loginType);    //绑定地方法接口

};
