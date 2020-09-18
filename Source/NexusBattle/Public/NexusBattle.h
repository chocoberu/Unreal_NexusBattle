// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(NexusBattle, Log, All);
#define NBLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define NBLOG_S(Verbosity) UE_LOG(NexusBattle, Verbosity, TEXT("%s"), *NBLOG_CALLINFO)
#define NBLOG(Verbosity, Format, ...) UE_LOG(NexusBattle, Verbosity, TEXT("%s %s"), *NBLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define NBCHECK(Expr, ...) { if(!(Expr)) {NBLOG(Error,TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__; } }

enum Team
{
	Red = 1,
	Blue = 2,
	Neutral = 3,
};