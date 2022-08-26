// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TUJsonHelper.h"
#include "XUResponseModel.h"
#include "XUBindModel.generated.h"

UENUM()
enum FXDGBindType
{
	Bind = 0,   //绑定
	UnBind = 1  //未绑定
};

USTRUCT()
struct FXUBindModel
{
	GENERATED_BODY()

	UPROPERTY()
	int loginType;

	UPROPERTY()
	FString loginName;

	UPROPERTY()
	FString nickName;

	UPROPERTY()
	FString avatar;

	UPROPERTY()
	FString bindDate;

	UPROPERTY()
	int status;

};

USTRUCT()
struct FXUBindResponseModel : public FXUResponseModel
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FXUBindModel> data;

};

