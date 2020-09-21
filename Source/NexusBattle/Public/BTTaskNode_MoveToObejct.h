// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NexusBattle.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_MoveToObejct.generated.h"

/**
 * 
 */
UCLASS()
class NEXUSBATTLE_API UBTTaskNode_MoveToObejct : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_MoveToObejct();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
