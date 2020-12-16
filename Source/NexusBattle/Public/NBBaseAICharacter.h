// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NexusBattle.h"
#include "NBBaseCharacter.h"
#include "NBBaseAICharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);

UCLASS()
class NEXUSBATTLE_API ANBBaseAICharacter : public ANBBaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANBBaseAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	float GetHPRatio();

	FOnHPChangedDelegate OnHPChanged;
protected:
	UPROPERTY(VisibleAnywhere, Category = UI) class UWidgetComponent* HPBarWidget; // HP Bar

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true)) bool IsAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true)) float AttackRange;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true)) float AttackRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, Meta = (AllowPrivateAccess = true)) float CurrentHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, Meta = (AllowPrivateAccess = true)) float MaxHP;
};
