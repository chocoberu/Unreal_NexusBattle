// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NexusBattle.h"
#include "Animation/AnimInstance.h"
#include "NBGunnerAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
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
	void PlayRocketDashFrontMontage();

	void SetDeadAnim() { IsDead = true; }

	UAnimMontage* GetNormalAttackMontage();

	FOnAttackHitCheckDelegate OnAttackHitCheck;
private:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* NAMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* RDFrontMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsDead;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsFire;
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrentSpeed;

private:
	UFUNCTION() void AnimNotify_NormalAttackCheck();
};
