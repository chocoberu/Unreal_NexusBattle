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

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		RDF_MONTAGE(TEXT("/Game/ParagonTwinblast/Characters/Heroes/TwinBlast/RocketDashFront_Montage.RocketDashFront_Montage"));

	if (RDF_MONTAGE.Succeeded())
	{
		RDFrontMontage = RDF_MONTAGE.Object;
	}

	IsDead = false;
	IsFire = false;
	CurrentSpeed = 0.0f;
}
void UNBGunnerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentSpeed = Pawn->GetVelocity().Size();
	}
	
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

void UNBGunnerAnimInstance::PlayRocketDashFrontMontage()
{
	Montage_Play(RDFrontMontage, 1.0f);
	NBLOG(Warning, TEXT("RocketDash End"));
}

UAnimMontage* UNBGunnerAnimInstance::GetNormalAttackMontage()
{
	return NAMontage;
}

FOnAttackHitCheckDelegate UNBGunnerAnimInstance::GetOnAttackHitCheck()
{
	return OnAttackHitCheck;
}

void UNBGunnerAnimInstance::AnimNotify_NormalAttackCheck()
{
	OnAttackHitCheck.Broadcast();
}
