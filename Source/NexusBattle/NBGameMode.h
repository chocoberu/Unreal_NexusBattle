// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NexusBattle.h"
#include "GameFramework/GameModeBase.h"
#include "NBGameMode.generated.h"

/**
 * 
 */
UCLASS()
class NEXUSBATTLE_API ANBGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ANBGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
};
