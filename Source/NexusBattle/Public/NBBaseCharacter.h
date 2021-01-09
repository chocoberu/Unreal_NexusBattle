// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NexusBattle.h"
#include "GameFramework/Character.h"
#include "NBBaseCharacter.generated.h"

UCLASS()
class NEXUSBATTLE_API ANBBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANBBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	

	ETeam GetMyTeam() { return MyTeam; }
	virtual void SetMyTeam(ETeam NewTeam) { MyTeam = NewTeam; }
	ECharacterState GetCharacterState() { return CharacterState; }
	void SetCharacterState(ECharacterState NewState) { CharacterState = NewState; }

	virtual void OnDead();
protected:
	
	// ÆÀ °ü·Ã
	UPROPERTY(EditAnywhere, Category = Team)
		ETeam MyTeam;

	UPROPERTY(EditAnywhere, Category = State)
		ECharacterState CharacterState;

	UPROPERTY(EditAnywhere, Category = Exp) float DropExp;
};
