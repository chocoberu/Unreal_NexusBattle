// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NexusBattle.h"
#include "NBBasePlayerCharacter.h"
#include "NBGunnerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class NEXUSBATTLE_API ANBGunnerCharacter : public ANBBasePlayerCharacter
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

	virtual void NormalAttack();
	virtual void RocketDash();

	UFUNCTION() void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	virtual void OnDead();

private:
	UPROPERTY() class UNBGunnerAnimInstance* GunnerAnim; // 애님 인스턴스

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true)) bool IsAttacking;

	UPROPERTY(VisibleAnywhere, Category = Attack, Meta = (AllowPrivateAccess = true)) class ANBGunnerHitEffect* HitEffectClass;

	// TODO : 캐릭터 상태를 나타낼 enum 추가

private:
	void NormalAttackCheck();

};
