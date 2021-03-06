// Fill out your copyright notice in the Description page of Project Settings.


#include "NBNexus.h"
#include "NBNormalMinion.h"
#include "Components/WidgetComponent.h"

// Sets default values
ANBNexus::ANBNexus()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("STATICMESH"));
	StaticMesh->SetupAttachment(GetMesh());

	GetCapsuleComponent()->SetCapsuleHalfHeight(172.0f);
	GetCapsuleComponent()->SetCapsuleRadius(138.0f);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -172.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ST_NEXUS(TEXT("/Game/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Statues/StaticMesh/SM_Plains_Angel_Statue_01.SM_Plains_Angel_Statue_01"));

	if (ST_NEXUS.Succeeded())
	{
		StaticMesh->SetStaticMesh(ST_NEXUS.Object);
	}
	// 콜리전 설정
	//GetCapsuleComponent()->SetCollisionProfileName(TEXT("NBCharacter"));

	CurrentHP = MaxHP = 2000.0f;

	SpawnInterval = 10.0f;
}

// Called when the game starts or when spawned
void ANBNexus::BeginPlay()
{
	Super::BeginPlay();
	

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ANBNexus::SpawnMinion, SpawnInterval, true);

	SpawnMinion();
}

void ANBNexus::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
}

// Called every frame
void ANBNexus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANBNexus::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ANBNexus::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CurrentHP = FMath::Clamp<float>(CurrentHP - FinalDamage, 0.0f, MaxHP);
	OnHPChanged.Broadcast();
	NBLOG(Warning, TEXT("Nexus Current HP : %f"), CurrentHP);

	if (CurrentHP <= 0.0f)
	{
		// 넥서스 파괴 시 미니언 스폰 중지
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);

		SetActorEnableCollision(false);
		HPBarWidget->SetHiddenInGame(true);
	}
	
	return FinalDamage;
}

void ANBNexus::SpawnMinion()
{
	MinionClass = GetWorld()->SpawnActor<ANBNormalMinion>(ANBNormalMinion::StaticClass(), GetActorLocation() + GetActorForwardVector() * 5.0f, GetActorRotation());
	MinionClass->SetMyTeam(this->GetMyTeam());
}

