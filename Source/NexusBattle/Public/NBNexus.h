// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NexusBattle.h"
#include "NBBaseAICharacter.h"
#include "NBNexus.generated.h"

UCLASS()
class NEXUSBATTLE_API ANBNexus : public ANBBaseAICharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ANBNexus();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere, Category = Visual) UStaticMeshComponent* StaticMesh;
	
};
