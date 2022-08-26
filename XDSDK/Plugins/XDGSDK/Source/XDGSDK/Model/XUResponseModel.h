// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TUJsonHelper.h"
#include "XUResponseModel.generated.h"

// {"code":50000,"msg":"Unknown Error","detail":"Required String parameter 'clientId' is not present","data":""}
USTRUCT()
struct FXUResponseModel
{
	GENERATED_BODY()

	UPROPERTY()
	int code;
	
	UPROPERTY()
	FString msg;

	UPROPERTY()
	FString detail;

};

