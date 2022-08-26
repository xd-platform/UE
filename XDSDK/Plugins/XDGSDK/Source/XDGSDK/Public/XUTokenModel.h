// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XUTokenModel.generated.h"


USTRUCT()
struct FXUTokenModel
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString kid;

	UPROPERTY()
	FString macKey;

	UPROPERTY()
	FString tokenType;

	UPROPERTY()
	int64 expireIn;  //游客才有

	UPROPERTY()
	FString macAlgorithm; //tapTap才有

	void SaveToLocal();

	static TSharedPtr<FXUTokenModel> GetLocalModel();

	static void ClearToken();
	
};

USTRUCT()
struct FXUSyncTokenModel
{
	GENERATED_BODY()

	UPROPERTY()
	FString sessionToken;

	void SaveToLocal();

	static TSharedPtr<FXUSyncTokenModel> GetLocalModel();

	static void ClearToken();
};



