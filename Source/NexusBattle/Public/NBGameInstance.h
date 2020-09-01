// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NexusBattle.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "NBGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FNBCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FNBCharacterData() : Level(1), MaxHP(100.0f), Attack(10.0f), Defence(5.0f), Speed(700.0f), Skill1(0.0f), Skill2(0.0f), Skill3(0.0f), Skill4(0.0f), DropExp(10), NextExp(30) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data") int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data") float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data") float Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data") float Defence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data") float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data") float Skill1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data") float Skill2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data") float Skill3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data") float Skill4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data") int32 DropExp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data") int32 NextExp;
};
/**
 * 
 */
UCLASS()
class NEXUSBATTLE_API UNBGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UNBGameInstance();

	virtual void Init() override;

	// 캐릭터 데이터 관련
	FNBCharacterData* GetGunneData(int32 Level);

private:
	UPROPERTY() class UDataTable* NBGunnerTable;
};
