// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NexusBattle.h"
#include "Blueprint/UserWidget.h"
#include "NBMinionWidget.generated.h"

/**
 * 
 */
UCLASS()
class NEXUSBATTLE_API UNBMinionWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void BindMinion(class ANBBaseAICharacter* NewMinion);
protected:
	virtual void NativeConstruct() override;
	void UpdateWidget();

private:
	UPROPERTY() class ANBBaseAICharacter* Minion;
	UPROPERTY() class UProgressBar* HPProgressBar;
};
