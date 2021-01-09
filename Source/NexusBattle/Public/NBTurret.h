// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NexusBattle.h"
#include "NBBaseAICharacter.h"
#include "NBTurret.generated.h"

UCLASS()
class NEXUSBATTLE_API ANBTurret : public ANBBaseAICharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANBTurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void SearchEnemy();

	virtual void OnDead() override;
	void ClearTimer();
private:
	UPROPERTY(VisibleAnywhere, Category = "Projectile", Meta = (AllowPrivateAccess = true)) class ANBTurretProjectile* ProjectileClass;

	UPROPERTY() FTimerHandle SearchTimerHandle;
	float SearchRange;
};
