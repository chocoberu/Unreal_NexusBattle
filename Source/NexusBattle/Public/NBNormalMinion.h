// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NexusBattle.h"
#include "NBBaseAICharacter.h"
#include "NBNormalMinion.generated.h"

UCLASS()
class NEXUSBATTLE_API ANBNormalMinion : public ANBBaseAICharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANBNormalMinion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual void SetMyTeam(ETeam NewTeam) override;

	void NormalAttack();
	void AttackStartComboState();
	void AttackEndComboState();
	
	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	virtual void OnDead();

private:
	UPROPERTY() class UNBMinionAnimInstance* MinionAnim; // ¾Ö´Ô ÀÎ½ºÅÏ½º
	//UPROPERTY(VisibleAnywhere, Category = UI) class UWidgetComponent* HPBarWidget; // HP Bar

	//UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true)) bool IsAttacking;

	//UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true)) float AttackRange;
	//UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true)) float AttackRadius;

	//UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Health, Meta = (AllowPrivateAccess = true)) float CurrentHP;

	// ÄÞº¸ °ø°Ý °ü·Ã
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true)) bool CanNextCombo;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true)) bool IsComboInputOn;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true)) int32 CurrentCombo;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true)) int32 MaxCombo;

	// AI
	UPROPERTY() class ANBMinionAIController* MinionAIController;

	// ½ºÄÌ·¹Å» ¸Þ½Ã ÄÄÆ÷³ÍÆ® 2
	UPROPERTY(EditAnywhere, Category = TEST)
		USkeletalMesh* BlueMesh;

private:
	void NormalAttackCheck();

};
