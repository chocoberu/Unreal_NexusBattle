// Fill out your copyright notice in the Description page of Project Settings.


#include "NBBaseAICharacter.h"
#include "Components/WidgetComponent.h"
#include "NBMinionWidget.h"

// Sets default values
ANBBaseAICharacter::ANBBaseAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	HPBarWidget->SetupAttachment(GetMesh());

	// HP Bar
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget>
		UI_HUD(TEXT("/Game/UI/UI_HPBar_Minion.UI_HPBar_Minion_C"));

	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}

	MaxHP = CurrentHP = 100.0f;
}

// Called when the game starts or when spawned
void ANBBaseAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	auto MinionWidget = Cast<UNBMinionWidget>(HPBarWidget->GetUserWidgetObject());
	if (MinionWidget != nullptr)
	{
		MinionWidget->BindMinion(this);
	}
}

// Called every frame
void ANBBaseAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANBBaseAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ANBBaseAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

float ANBBaseAICharacter::GetHPRatio()
{
	return (CurrentHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / MaxHP);
}

