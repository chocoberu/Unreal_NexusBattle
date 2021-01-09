// Fill out your copyright notice in the Description page of Project Settings.

#include "NBBaseCharacter.h"
#include "NBPlayerController.h"
#include "DrawDebugHelpers.h"
#include "NBCharacterStatComponent.h"
#include "Components/WidgetComponent.h"
#include "NBCharacterWidget.h"

// Sets default values
ANBBaseCharacter::ANBBaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyTeam = ETeam::Neutral;
	CharacterState = ECharacterState::Normal;

	// 콜리전 설정
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NBCharacter"));
}

// Called when the game starts or when spawned
void ANBBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ANBBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

// Called to bind functionality to input
void ANBBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
}

void ANBBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}
void ANBBaseCharacter::OnDead()
{
	SetCharacterState(ECharacterState::Dead);

}

