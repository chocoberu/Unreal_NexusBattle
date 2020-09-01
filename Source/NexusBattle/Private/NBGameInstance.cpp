// Fill out your copyright notice in the Description page of Project Settings.


#include "NBGameInstance.h"

UNBGameInstance::UNBGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable>
		DT_GUNNER(TEXT("/Game/GameData/GunnerData.GunnerData"));
	if (DT_GUNNER.Succeeded())
		NBGunnerTable = DT_GUNNER.Object;

	else
		NBLOG(Error, TEXT("Gunner Data load failed"));

	NBCHECK(NBGunnerTable->GetRowMap().Num() > 0);
}

void UNBGameInstance::Init()
{
	Super::Init();

	NBLOG(Warning, TEXT("Speed of Level 4 Gunner : %f"), GetGunneData(4)->Speed);
}

FNBCharacterData* UNBGameInstance::GetGunneData(int32 Level)
{
	return NBGunnerTable->FindRow<FNBCharacterData>(*FString::FromInt(Level), TEXT(""));
}
