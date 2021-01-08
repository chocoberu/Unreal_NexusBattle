// Fill out your copyright notice in the Description page of Project Settings.


#include "NBTurret.h"
#include "NBTurretProjectile.h"
#include "DrawDebugHelpers.h"

// Sets default values
ANBTurret::ANBTurret()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_TURRET(TEXT("/Game/ParagonProps/Turrets/Turrets_inhibitors/Meshes/Turret_Dark_GDC.Turret_Dark_GDC"));

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

	if (SK_TURRET.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_TURRET.Object);
	}
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	// 애님 인스턴스
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		TURRET_ANIM(TEXT("/Game/ParagonProps/Turrets/Turrets_inhibitors/Animations/Turret_AnimBP.Turret_AnimBP_C"));

	if (TURRET_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(TURRET_ANIM.Class);
	}

	// 탐색 관련
	SearchRange = 500.0f;
}

// Called when the game starts or when spawned
void ANBTurret::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(SearchTimerHandle, this, &ANBTurret::SearchEnemy, 5.0f, true);
	SearchEnemy();
}

void ANBTurret::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(SearchTimerHandle);
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
		GetActorLocation(), // 시작점
		GetActorLocation() + GetActorForwardVector() * SearchRange, // 끝점
		FQuat::Identity, // 회전값
		ECollisionChannel::ECC_GameTraceChannel2, // 트레이스채널
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

			// TODO : 포탑 muzzle 부분에서 생성되도록 수정
			ProjectileClass = GetWorld()->SpawnActor<ANBTurretProjectile>(ANBTurretProjectile::StaticClass(),
				GetActorLocation() + GetActorForwardVector() * 5.0f, GetActorRotation());
			ProjectileClass->ShotToTarget(HitResult.Actor.Get());
		}
		
	}
	else
		NBLOG(Warning, TEXT("No Target"));
}

