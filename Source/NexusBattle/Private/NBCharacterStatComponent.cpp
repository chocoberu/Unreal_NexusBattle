// Fill out your copyright notice in the Description page of Project Settings.


#include "NBCharacterStatComponent.h"
#include "NBGameInstance.h"
#include "NBGunnerCharacter.h"

// Sets default values for this component's properties
UNBCharacterStatComponent::UNBCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	Level = 1;
}


// Called when the game starts
void UNBCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UNBCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetNewLevel(Level);
}


// Called every frame
void UNBCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UNBCharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	auto NBGameInstance = Cast<UNBGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	NBCHECK(NBGameInstance != nullptr);
	
	// TODO : 다른 캐릭터 부분도 추가
	if (GetOwner()->GetClass() == ANBGunnerCharacter::StaticClass())
	{
		auto Character = Cast<ANBGunnerCharacter>(GetOwner());

		CurrentStatData = NBGameInstance->GetGunneData(NewLevel);
		if (CurrentStatData != nullptr)
		{
			Level = NewLevel;
			CurrentHP = CurrentStatData->MaxHP;
			NBLOG(Warning, TEXT("Gunner Max HP : %f"), CurrentStatData->MaxHP);
		}
		else
			NBLOG(Error, TEXT("Gunner Data doesn't exist"));
	}
	else
		NBLOG(Error, TEXT("DATA FAILED!!!"));
}

void UNBCharacterStatComponent::SetDamage(float NewDamage)
{
	NBCHECK(CurrentStatData != nullptr);
	CurrentHP = FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP);

	if (CurrentHP <= 0.0f)
		OnHPIsZero.Broadcast();
}

float UNBCharacterStatComponent::GetAttack()
{
	NBCHECK(CurrentStatData != nullptr, 0.0f);
	return CurrentStatData->Attack;
}

float UNBCharacterStatComponent::GetDefence()
{
	NBCHECK(CurrentStatData != nullptr, 0.0f);
	return CurrentStatData->Defence;
}

float UNBCharacterStatComponent::GetSpeed()
{
	NBCHECK(CurrentStatData != nullptr, 0.0f);
	return CurrentStatData->Speed;
}

float UNBCharacterStatComponent::GetSkill1()
{
	NBCHECK(CurrentStatData != nullptr, 0.0f);
	return CurrentStatData->Skill1;
}

float UNBCharacterStatComponent::GetSkill2()
{
	NBCHECK(CurrentStatData != nullptr, 0.0f);
	return CurrentStatData->Skill2;
}

float UNBCharacterStatComponent::GetSkill3()
{
	NBCHECK(CurrentStatData != nullptr, 0.0f);
	return CurrentStatData->Skill3;
}

float UNBCharacterStatComponent::GetSkill4()
{
	NBCHECK(CurrentStatData != nullptr, 0.0f);
	return CurrentStatData->Skill4;
}

float UNBCharacterStatComponent::GetCurrentHP()
{
	NBCHECK(CurrentStatData != nullptr, 0.0f);
	return CurrentHP;
}