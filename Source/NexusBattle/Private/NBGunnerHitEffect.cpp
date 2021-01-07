// Fill out your copyright notice in the Description page of Project Settings.


#include "NBGunnerHitEffect.h"

// Sets default values
ANBGunnerHitEffect::ANBGunnerHitEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HitParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HitParticle"));

	RootComponent = HitParticle;

	// 파티클 시스템
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		PT_HIT_PARTICLE(TEXT("/Game/ParagonTwinblast/FX/Particles/Abilities/Nitro/FX/P_TwinBlast_Nitro_HitCharacter.P_TwinBlast_Nitro_HitCharacter"));

	if (PT_HIT_PARTICLE.Succeeded())
	{
		HitParticle->SetTemplate(PT_HIT_PARTICLE.Object);
	}
}

// Called when the game starts or when spawned
void ANBGunnerHitEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANBGunnerHitEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANBGunnerHitEffect::PlayHitEffect()
{
	HitParticle->Activate(true);
	SetLifeSpan(1.0f);
}