// Fill out your copyright notice in the Description page of Project Settings.


#include "NBTurret.h"
#include "NBTurretProjectile.h"
#include "Components/WidgetComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ANBTurret::ANBTurret()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_TURRET(TEXT("/Game/ParagonProps/Turrets/Turrets_inhibitors/Meshes/Turret_Dark_GDC.Turret_Dark_GDC"));

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -180.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	GetCapsuleComponent()->InitCapsuleSize(100.0f, 200.0f);

	if (SK_TURRET.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_TURRET.Object);
	}
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	// �ִ� �ν��Ͻ�
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		TURRET_ANIM(TEXT("/Game/ParagonProps/Turrets/Turrets_inhibitors/Animations/Turret_AnimBP.Turret_AnimBP_C"));

	if (TURRET_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(TURRET_ANIM.Class);
	}

	// HPBar ��ġ ����
	if(HPBarWidget != nullptr)
		HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 300.0f));

	// Ž�� ����
	SearchRange = 500.0f;

	// HP 
	MaxHP = CurrentHP = 1500.0f;
}

// Called when the game starts or when spawned
void ANBTurret::BeginPlay()
{
	Super::BeginPlay();
	CurrentHP = MaxHP;
	
	GetWorld()->GetTimerManager().SetTimer(SearchTimerHandle, this, &ANBTurret::SearchEnemy, 5.0f, true);
	SearchEnemy();
}

void ANBTurret::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	ClearTimer();
}

// Called every frame
void ANBTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANBTurret::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ANBTurret::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	
	return FinalDamage;
}

void ANBTurret::SearchEnemy()
{
	NBLOG(Warning, TEXT("SearchEnemy() Call"));
	FHitResult HitResult;

	FCollisionQueryParams Params(NAME_None, false, this);

	for (TObjectIterator<ANBBaseCharacter> It; It; ++It)
	{
		if ((*It)->GetMyTeam() == MyTeam)
		{
			Params.AddIgnoredActor(*It);
		}
	}

	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(), // ������
		GetActorLocation() + GetActorForwardVector() * SearchRange, // ����
		FQuat::Identity, // ȸ����
		ECollisionChannel::ECC_GameTraceChannel2, // Ʈ���̽�ä��
		FCollisionShape::MakeSphere(SearchRange),
		Params);

#if ENABLE_DRAW_DEBUG

	FVector TraceVec = GetActorForwardVector() * SearchRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = SearchRange * 0.5f + SearchRange;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 1.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		SearchRange,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);
#endif
	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			NBLOG(Warning, TEXT("Target Actor Name : %s"), *HitResult.Actor->GetName());
			auto NBBaseCharacter = Cast<ANBBaseCharacter>(HitResult.Actor.Get());
			if (NBBaseCharacter == nullptr)
				return;
			if (NBBaseCharacter->GetMyTeam() == MyTeam && MyTeam != ETeam::Neutral)
				return;

			// TODO : ��ž muzzle �κп��� �����ǵ��� ����
			ProjectileClass = GetWorld()->SpawnActor<ANBTurretProjectile>(ANBTurretProjectile::StaticClass(),
				GetActorLocation() + GetActorForwardVector() * 10.0f, GetActorRotation());
			ProjectileClass->ShotToTarget(HitResult.Actor.Get(), GetController());
		}
		
	}
	else
		NBLOG(Warning, TEXT("No Target"));
}
void ANBTurret::OnDead()
{
	Super::OnDead();
	SetActorEnableCollision(true);
	ClearTimer();
	NBLOG(Warning, TEXT("Turret Dead, Clear Timer Called"));
}

void ANBTurret::ClearTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(SearchTimerHandle);
}