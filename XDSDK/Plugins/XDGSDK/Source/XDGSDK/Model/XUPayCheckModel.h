// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XUPayCheckModel.generated.h"


USTRUCT()
struct FXUPayCheckModel
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString tradeNo;

	UPROPERTY()
	FString outTradeNo;

	UPROPERTY()
	FString productId;

	UPROPERTY()
	FString currency;

	UPROPERTY()
	FString channelType;

	UPROPERTY()
	FString channelId;

	UPROPERTY()
	int64 supplyStatus;

	UPROPERTY()
	int64 platform;

	UPROPERTY()
	double refundAmount;

};


USTRUCT()
struct FXUPayCheckResponseModel
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FXUPayCheckModel> list;

};

