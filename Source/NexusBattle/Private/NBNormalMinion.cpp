// Fill out your copyright notice in the Description page of Project Settings.


#include "NBNormalMinion.h"
#include "NBMinionAnimInstance.h"

// Sets default values
ANBNormalMinion::ANBNormalMinion()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_MINION(TEXT("/Game/ParagonMinions/Characters/Minions/Down_Minions/Meshes/Minion_Lane_Melee_Dawn.Minion_Lane_Melee_Dawn"));

	if (SK_MINION.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_MINION.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance>
		MINION_ANIM(TEXT("/Game/Animation/NormalMinionAnimBlueprint.NormalMinionAnimBlueprint_C"));

	if (MINION_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(MINION_ANIM.Class);
	}

	// 콜리전 설정
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NBCharacter"));

	// HP
	CurrentHP = 100.0f;
}

// Called when the game starts or when spawned
void ANBNormalMinion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANBNormalMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANBNormalMinion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANBNormalMinion::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MinionAnim = Cast<UNBMinionAnimInstance>(GetMesh()->GetAnimInstance());

}

float ANBNormalMinion::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CurrentHP = FMath::Clamp<float>(CurrentHP - FinalDamage, 0.0f, 100.0f);

	NBLOG(Warning, TEXT("Minion Current HP : %f"), CurrentHP);
	// 테스트 코드
	// TODO : hpbar, 공격 추가
	if (CurrentHP <= 0.0f)
	{
		MinionAnim->SetDeadAnim();
		SetActorEnableCollision(false);
	}
	return FinalDamage;
}
