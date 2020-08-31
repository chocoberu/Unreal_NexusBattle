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

	UFUNCTION() void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	UPROPERTY() class UNBGunnerAnimInstance* GunnerAnim; // �ִ� �ν��Ͻ�

	// �� �߻��� ���� ��ƼŬ �ý��� (Normal Attack)
	UPROPERTY(VisibleAnywhere, Category = Particle) UParticleSystemComponent* LeftMuzzleParticle; 
	UPROPERTY(VisibleAnywhere, Category = Particle) UParticleSystemComponent* RightMuzzleParticle;

	// ���� ��� ��ƼŬ �ý���
	UPROPERTY(VisibleAnywhere, Category = Particle) UParticleSystemComponent* LeftRocketDashParticle;
	UPROPERTY(VisibleAnywhere, Category = Particle) UParticleSystemComponent* RightRocketDashParticle;


	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true)) bool IsAttacking;

	// TODO : ĳ���� ���¸� ��Ÿ�� enum �߰�

private:
	void NormalAttackCheck();

};
