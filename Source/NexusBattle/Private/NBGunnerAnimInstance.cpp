// Fill out your copyright notice in the Description page of Project Settings.


#include "NBGunnerAnimInstance.h"

UNBGunnerAnimInstance::UNBGunnerAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		NA_MONTAGE(TEXT("/Game/ParagonTwinblast/Characters/Heroes/TwinBlast/Animations/Primary_Fire_Med_A_Montage.Primary_Fire_Med_A_Montage"));

	if (NA_MONTAGE.Succeeded())
	{
		NAMontage = NA_MONTAGE.Object;
	}

	IsDead = false;
	IsFire = false;
}
void UNBGunnerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UNBGunnerAnimInstance::PlayNormalAttackMontage()
{
	NBCHECK(!IsDead);
	if (!IsFire)
	{
		Montage_Play(NAMontage, 1.0f);
		//IsFire = true;
	}
}
