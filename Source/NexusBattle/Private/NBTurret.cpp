// Fill out your copyright notice in the Description page of Project Settings.


#include "NBTurret.h"

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
}

// Called when the game starts or when spawned
void ANBTurret::BeginPlay()
{
	Super::BeginPlay();
	
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

