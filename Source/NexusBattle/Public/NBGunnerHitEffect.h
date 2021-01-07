// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NexusBattle.h"
#include "GameFramework/Actor.h"
#include "NBGunnerHitEffect.generated.h"

UCLASS()
class NEXUSBATTLE_API ANBGunnerHitEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANBGunnerHitEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = Particle) UParticleSystemComponent* HitParticle;

	void PlayHitEffect();

};
