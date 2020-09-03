// Fill out your copyright notice in the Description page of Project Settings.


#include "NBMinionAnimInstance.h"

UNBMinionAnimInstance::UNBMinionAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACK_MONTAGE(TEXT("/Game/Animation/MinionAttackMontage.MinionAttackMontage"));

	if (ATTACK_MONTAGE.Succeeded())
	{
		NAMontage = ATTACK_MONTAGE.Object;
	}

	IsDead = false;
	IsAttack = false;
	CurrentSpeed = 0.0f;
}

void UNBMinionAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentSpeed = Pawn->GetVelocity().Size();
	}
}

void UNBMinionAnimInstance::PlayNormalAttackMontage()
{
	NBCHECK(!IsDead);
	NBLOG(Warning, TEXT("Minion Normal Attack Montage"));
	Montage_Play(NAMontage, 0.8f);
	
}
void UNBMinionAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	NBCHECK(Montage_IsPlaying(NAMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), NAMontage);
}
void UNBMinionAnimInstance::AnimNotify_AttackCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UNBMinionAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

FName UNBMinionAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	NBCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
