// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NBMinionAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NEXUSBATTLE_API UNBMinionAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UNBMinionAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void PlayNormalAttackMontage();
	void SetDeadAnim() { IsDead = true; }

private:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* NAMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsDead;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsAttack;
};
