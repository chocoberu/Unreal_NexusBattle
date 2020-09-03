// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NexusBattle.h"
#include "AIController.h"
#include "NBMinionAIController.generated.h"

/**
 * 
 */
UCLASS()
class NEXUSBATTLE_API ANBMinionAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ANBMinionAIController();

	virtual void OnPossess(APawn* InPawn) override;

	void RunAI(); // AI 가동
	void StopAI(); // AI 작동 중단

private:
	UPROPERTY() class UBehaviorTree* BTAsset;
	UPROPERTY() class UBlackboardData* BBAsset;
};
