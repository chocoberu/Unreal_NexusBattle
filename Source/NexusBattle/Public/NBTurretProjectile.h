// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NexusBattle.h"
#include "GameFramework/Actor.h"
#include "NBTurretProjectile.generated.h"

UCLASS()
class NEXUSBATTLE_API ANBTurretProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANBTurretProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ��ü �ݸ��� ������Ʈ
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile) USphereComponent* CollisionComponent;

	// ��ƼŬ ����
	UPROPERTY(VisibleAnywhere, Category = Particle) UParticleSystemComponent* ProjectileParticleSystem;
	UPROPERTY(VisibleAnywhere, Category = Particle) UParticleSystem* HitParticle;
	
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);


	void ShotToTarget(class AActor* Target);
	void SetHitParticle();
	
	class AActor* TargetActor;

	float Speed = 500.0f;
};
