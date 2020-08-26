// Fill out your copyright notice in the Description page of Project Settings.


#include "NBGunnerCharacter.h"
#include "NBGunnerAnimInstance.h"

ANBGunnerCharacter::ANBGunnerCharacter()
{
	// 컴포넌트 생성
	LeftMuzzleParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LMUZZLEPARTICLE"));
	RightMuzzleParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RMUZZLEPARTICLE"));
	
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	
	//
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_GUNNER(TEXT("/Game/ParagonTwinblast/Characters/Heroes/TwinBlast/Meshes/TwinBlast.TwinBlast"));

	if (SK_GUNNER.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_GUNNER.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);


	FName MuzzleSocketL(TEXT("Muzzle_02"));
	FName MuzzleSocketR(TEXT("Muzzle_01"));

	LeftMuzzleParticle->SetupAttachment(GetMesh(), MuzzleSocketL);
	RightMuzzleParticle->SetupAttachment(GetMesh(), MuzzleSocketR);

	static ConstructorHelpers::FClassFinder<UAnimInstance>
		GUNNER_ANIM(TEXT("/Game/ParagonTwinblast/Characters/Heroes/TwinBlast/GunnerAnimBlueprint.GunnerAnimBlueprint_C"));

	if (GUNNER_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(GUNNER_ANIM.Class);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		PT_MUZZLE_FLASH_LEFT(TEXT("/Game/ParagonTwinblast/FX/Particles/Abilities/Primary/FX/P_TwinBlast_Primary_MuzzleFlashLeft.P_TwinBlast_Primary_MuzzleFlashLeft"));

	if (PT_MUZZLE_FLASH_LEFT.Succeeded())
	{
		LeftMuzzleParticle->SetTemplate(PT_MUZZLE_FLASH_LEFT.Object);
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		PT_MUZZLE_FLASH_RIGHT(TEXT("/Game/ParagonTwinblast/FX/Particles/Abilities/Primary/FX/P_TwinBlast_Primary_MuzzleFlash.P_TwinBlast_Primary_MuzzleFlash"));

	if (PT_MUZZLE_FLASH_LEFT.Succeeded())
	{
		RightMuzzleParticle->SetTemplate(PT_MUZZLE_FLASH_LEFT.Object);
	}
	LeftMuzzleParticle->bAllowRecycling = true;
	RightMuzzleParticle->bAllowRecycling = true; // Allow Recycling 활성화

}
void ANBGunnerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ANBGunnerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANBGunnerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("NormalAttack"), EInputEvent::IE_Pressed, this, &ANBGunnerCharacter::NormalAttack);
}

void ANBGunnerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GunnerAnim = Cast<UNBGunnerAnimInstance>(GetMesh()->GetAnimInstance());
	NBCHECK(GunnerAnim != nullptr);
}

void ANBGunnerCharacter::NormalAttack()
{
	// TODO
	GunnerAnim->PlayNormalAttackMontage();

	// 파티클 테스트
	LeftMuzzleParticle->Activate(true);
	RightMuzzleParticle->Activate(true);
}