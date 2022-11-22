// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TUJsonHelper.h"
#include "XUError.generated.h"

USTRUCT(BlueprintType)
struct FXUError
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int code = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString msg;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString detail;
	
	TSharedPtr<FJsonObject> ExtraData;

	bool IsNetWorkError = false;

	FString GetExtraString() {
		return TUJsonHelper::GetJsonString(ExtraData);
	}

	FXUError() = default;
	FXUError(const FString& message) :code(-1), msg(message) {};
};

