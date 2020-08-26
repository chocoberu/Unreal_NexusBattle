// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NBBaseCharacter.h"
#include "NBGunnerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class NEXUSBATTLE_API ANBGunnerCharacter : public ANBBaseCharacter
{
	GENERATED_BODY()
	
public:
	ANBGunnerCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

	void NormalAttack();
	void RocketDash();

private:
	UPROPERTY() class UNBGunnerAnimInstance* GunnerAnim; // 애님 인스턴스
	UPROPERTY(VisibleAnywhere, Category = Particle) UParticleSystemComponent* LeftMuzzleParticle; // 총 발사할 때의 파티클 시스템
	UPROPERTY(VisibleAnywhere, Category = Particle) UParticleSystemComponent* RightMuzzleParticle; // 총 발사할 때의 파티클 시스템
};
