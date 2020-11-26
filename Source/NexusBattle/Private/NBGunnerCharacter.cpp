// Fill out your copyright notice in the Description page of Project Settings.


#include "NBGunnerCharacter.h"
#include "NBGunnerAnimInstance.h"
#include "DrawDebugHelpers.h"
#include "NBCharacterStatComponent.h"
#include "Components/WidgetComponent.h"
#include "NBCharacterWidget.h"
#include "NBPlayerController.h"

ANBGunnerCharacter::ANBGunnerCharacter()
{
	// ������Ʈ ����
	LeftMuzzleParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LMUZZLEPARTICLE"));
	RightMuzzleParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RMUZZLEPARTICLE"));

	LeftRocketDashParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LROCKET_PARTICLE"));
	RightRocketDashParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RROCKET_PARTICLE"));
	
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	
	//
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_GUNNER(TEXT("/Game/ParagonTwinblast/Characters/Heroes/TwinBlast/Meshes/TwinBlast.TwinBlast"));

	if (SK_GUNNER.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_GUNNER.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	// ��ƼŬ �ý��� ����
	FName MuzzleSocketL(TEXT("Muzzle_02"));
	FName MuzzleSocketR(TEXT("Muzzle_01"));
	FName RocketDashL(TEXT("FX_Booster_L_Mid"));
	FName RocketDashR(TEXT("FX_Booster_R_Mid"));

	LeftMuzzleParticle->SetupAttachment(GetMesh(), MuzzleSocketL);
	RightMuzzleParticle->SetupAttachment(GetMesh(), MuzzleSocketR);
	LeftRocketDashParticle->SetupAttachment(GetMesh(), RocketDashL);
	RightRocketDashParticle->SetupAttachment(GetMesh(), RocketDashR);

	// �ִ� �ν��Ͻ�
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		GUNNER_ANIM(TEXT("/Game/ParagonTwinblast/Characters/Heroes/TwinBlast/GunnerAnimBlueprint.GunnerAnimBlueprint_C"));

	if (GUNNER_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(GUNNER_ANIM.Class);
	}

	// ��ƼŬ �ý���
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
	RightMuzzleParticle->bAllowRecycling = true; // Allow Recycling Ȱ��ȭ

	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		PT_ROCKET_DASH(TEXT("/Game/ParagonTwinblast/FX/Particles/Abilities/Dive/FX/P_DiveBooster_Arms.P_DiveBooster_Arms"));

	if (PT_ROCKET_DASH.Succeeded())
	{
		LeftRocketDashParticle->SetTemplate(PT_ROCKET_DASH.Object);
		RightRocketDashParticle->SetTemplate(PT_ROCKET_DASH.Object);
	}
	LeftRocketDashParticle->bAllowRecycling = true;
	RightRocketDashParticle->bAllowRecycling = true;

	IsAttacking = false;

	// ���� ����
	AttackRange = 300.0f;
	AttackRadius = 50.0f;
}
void ANBGunnerCharacter::BeginPlay()
{
	Super::BeginPlay();

	NBLOG(Warning, TEXT("gunner character beginPlay"));

	// ĳ���� �����Ʈ�� �ӵ� �׽�Ʈ
	GetCharacterMovement()->MaxWalkSpeed = 700.0f;
	GetCharacterMovement()->JumpZVelocity = 200.0f;
}

void ANBGunnerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ���� ĳ���� �ӵ� log
	//NBLOG(Warning, TEXT("Current Speed : %f"), GetCharacterMovement()->Velocity.Size());
}

void ANBGunnerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("NormalAttack"), EInputEvent::IE_Pressed, this, &ANBGunnerCharacter::NormalAttack);
	PlayerInputComponent->BindAction(TEXT("Skill1"), EInputEvent::IE_Pressed, this, &ANBGunnerCharacter::RocketDash);
}

void ANBGunnerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GunnerAnim = Cast<UNBGunnerAnimInstance>(GetMesh()->GetAnimInstance());
	NBCHECK(GunnerAnim != nullptr);

	GunnerAnim->OnMontageEnded.AddDynamic(this, &ANBGunnerCharacter::OnAttackMontageEnded);
	GunnerAnim->OnAttackHitCheck.AddUObject(this, &ANBGunnerCharacter::NormalAttackCheck);

	CharacterStat->OnHPIsZero.AddLambda([this]() -> void
		{
			NBLOG(Warning, TEXT("Gunner HP is zero"));
			SetCharacterState(ECharacterState::Dead);
			GunnerAnim->SetDeadAnim();
			SetActorEnableCollision(false);
			HPBarWidget->SetHiddenInGame(true);
		});
}

void ANBGunnerCharacter::NormalAttack()
{
	if (IsAttacking)
		return;

	// TODO
	GunnerAnim->PlayNormalAttackMontage();

	// ��ƼŬ �׽�Ʈ
	LeftMuzzleParticle->Activate(true);
	RightMuzzleParticle->Activate(true);
	IsAttacking = true;
}
void ANBGunnerCharacter::RocketDash()
{
	// TODO : ��� ���� ���� �ʿ�
	
	// TODO : ���� ����(���� �ӵ� ��)

	//NBLOG(Warning, TEXT("Call RocketDash"));

	// �׽�Ʈ �ڵ�
	float Velocity = 900.0f; // ���� �ӵ�
	GetCharacterMovement()->Velocity = GetActorForwardVector() * Velocity;
	Jump();
	GunnerAnim->PlayRocketDashFrontMontage();

	LeftRocketDashParticle->Activate(true);
	RightRocketDashParticle->Activate(true);
}

void ANBGunnerCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage != GunnerAnim->GetNormalAttackMontage())
	{
		//NBLOG(Warning, TEXT("Not Normal Attack"));
		return;
	}
	NBCHECK(IsAttacking);
	IsAttacking = false;
}

void ANBGunnerCharacter::NormalAttackCheck()
{
	NBLOG(Warning, TEXT("NormalAttack Check"));
	FHitResult HitResult;
	//TArray<FHitResult> HitResults;
	FCollisionQueryParams Params(NAME_None, false, this);

	for (TObjectIterator<ANBBaseCharacter> It; It; ++It)
	{
		if ((*It)->GetMyTeam() == MyTeam)
		{
			Params.AddIgnoredActor(*It);
			//NBLOG(Warning, TEXT("Same Team Actor : %s"), *(*It)->GetName());
		}
	}
	
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(), // ������
		GetActorLocation() + GetActorForwardVector() * AttackRange, // ����
		FQuat::Identity, // ȸ����
		ECollisionChannel::ECC_GameTraceChannel2, // Ʈ���̽�ä��
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

#if ENABLE_DRAW_DEBUG

	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 1.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);
#endif
	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			NBLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());
			auto NBBaseCharacter = Cast<ANBBaseCharacter>(HitResult.Actor.Get());
			if (NBBaseCharacter == nullptr)
				return;
			if (NBBaseCharacter->GetMyTeam() == MyTeam && MyTeam != ETeam::Neutral)
				return;

			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(CharacterStat->GetAttack(), // ������ ũ��
				DamageEvent, // ������ ����
				GetController(), // ������ (��Ʈ�ѷ�)
				this); // ������ ������ ���� ����� ���� (����)
			// TODO : Ÿ�� ��ƼŬ �߰�
		}
		/*NBLOG(Warning, TEXT("Total Hit Actors : %d"), HitResults.Num());
		for (auto& Result : HitResults)
		{
			if (Result.Actor.IsValid())
			{
				NBLOG(Warning, TEXT("Hit Actor Name : %s"), *Result.Actor->GetName());
			}
		}*/
	}
}
