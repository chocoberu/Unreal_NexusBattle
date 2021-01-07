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
			if (Level == 1)
			{
				SetHP(CurrentStatData->MaxHP);
				SetMP(CurrentStatData->MaxMP);
			}
			else
			{
				SetHP(CurrentHP + 10.0f);
				SetMP(CurrentMP + 20.0f);
				OnLevelChanged.Broadcast();
			}
			NBLOG(Warning, TEXT("Gunner Max HP : %f"), CurrentStatData->MaxHP);
		}
		else
			NBLOG(Error, TEXT("Gunner Data doesn't exist"));
	}
	else
		NBLOG(Error, TEXT("DATA FAILED!!!"));
}
void UNBCharacterStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();

	// 사망 처리
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}
void UNBCharacterStatComponent::SetMP(float NewMP)
{
	CurrentMP = NewMP;
	OnHPChanged.Broadcast();
}

void UNBCharacterStatComponent::SetDamage(float NewDamage)
{
	NBCHECK(CurrentStatData != nullptr);
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));
}
void UNBCharacterStatComponent::AddExp(float NewExp)
{
	CurrentExp += NewExp;
	NBLOG(Warning, TEXT("Current Exp : %f"), CurrentExp);

	// 레벨 업 처리
	if (CurrentExp >= GetNextExp())
	{
		CurrentExp -= GetNextExp();
		SetNewLevel(Level + 1);
	}
}

float UNBCharacterStatComponent::GetAttack() const
{
	NBCHECK(CurrentStatData != nullptr, 0.0f);
	return CurrentStatData->Attack;
}

float UNBCharacterStatComponent::GetDefence() const
{
	NBCHECK(CurrentStatData != nullptr, 0.0f);
	return CurrentStatData->Defence;
}

float UNBCharacterStatComponent::GetSpeed() const
{
	NBCHECK(CurrentStatData != nullptr, 0.0f);
	return CurrentStatData->Speed;
}

float UNBCharacterStatComponent::GetSkill1() const
{
	NBCHECK(CurrentStatData != nullptr, 0.0f);
	return CurrentStatData->Skill1;
}

float UNBCharacterStatComponent::GetSkill2() const
{ 
	NBCHECK(CurrentStatData != nullptr, 0.0f);
	return CurrentStatData->Skill2;
}

float UNBCharacterStatComponent::GetSkill3() const
{
	NBCHECK(CurrentStatData != nullptr, 0.0f);
	return CurrentStatData->Skill3;
}

float UNBCharacterStatComponent::GetSkill4() const
{
	NBCHECK(CurrentStatData != nullptr, 0.0f);
	return CurrentStatData->Skill4;
}

float UNBCharacterStatComponent::GetCurrentHP() const
{
	NBCHECK(CurrentStatData != nullptr, 0.0f);
	return CurrentHP;
}

float UNBCharacterStatComponent::GetHPRatio() const
{
	NBCHECK(CurrentStatData != nullptr, 0.0f);

	return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / CurrentStatData->MaxHP);
}
float UNBCharacterStatComponent::GetMPRatio() const
{
	NBCHECK(CurrentStatData != nullptr, 0.0f);

	return (CurrentStatData->MaxMP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentMP / CurrentStatData->MaxMP);
}

float UNBCharacterStatComponent::GetNextExp() const
{
	NBCHECK(CurrentStatData != nullptr, 0.0f);
	return CurrentStatData->NextExp;
}
int32 UNBCharacterStatComponent::GetLevel() const
{
	return Level;
}