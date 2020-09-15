// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NexusBattle.h"
#include "Animation/AnimInstance.h"
#include "NBTurretAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NEXUSBATTLE_API UNBTurretAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UNBTurretAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
