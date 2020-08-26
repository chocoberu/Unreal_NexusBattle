// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NexusBattle.h"
#include "Animation/AnimInstance.h"
#include "NBGunnerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NEXUSBATTLE_API UNBGunnerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UNBGunnerAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds);
	void PlayNormalAttackMontage();

	void SetDeadAnim() { IsDead = true; }

private:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* NAMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsDead;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsFire;
	
};
