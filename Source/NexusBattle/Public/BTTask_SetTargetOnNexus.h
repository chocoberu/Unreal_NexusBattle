// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NexusBattle.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetTargetOnNexus.generated.h"

/**
 * 
 */
UCLASS()
class NEXUSBATTLE_API UBTTask_SetTargetOnNexus : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_SetTargetOnNexus();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
