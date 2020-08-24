// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NexusBattle.h"
#include "GameFramework/PlayerController.h"
#include "NBPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class NEXUSBATTLE_API ANBPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;
};
