// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NexusBattle.h"
#include "Blueprint/UserWidget.h"
#include "NBCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class NEXUSBATTLE_API UNBCharacterWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(class UNBCharacterStatComponent* NewCharacterStat);
	
protected:
	virtual void NativeConstruct() override;
	void UpdateWidget();

private:
	UPROPERTY() class UNBCharacterStatComponent* CurrentCharacterStat;
	UPROPERTY() class UProgressBar* HPProgressBar;
	UPROPERTY() class UProgressBar* MPProgressBar;
	UPROPERTY() class UTextBlock* LevelText;
};
