// Fill out your copyright notice in the Description page of Project Settings.


#include "NBMinionAnimInstance.h"

UNBMinionAnimInstance::UNBMinionAnimInstance()
{
	IsDead = false;
	IsAttack = false;
}

void UNBMinionAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UNBMinionAnimInstance::PlayNormalAttackMontage()
{
	
}
