// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogXDDemo, All, All);

#define DEMO_LOG(Format, ...) \
	if (GEngine) \
	{ \
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(Format, ##__VA_ARGS__)); \
	}\
	UE_LOG(LogXDDemo, Log, Format, ##__VA_ARGS__)

#define DEMO_LOG_STRING(String) \
	if (GEngine) \
	{ \
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, String); \
	}\
	UE_LOG(LogXDDemo, Log, TEXT("%s"), *String)
