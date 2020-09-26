// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToNexus.generated.h"

/**
 * 
 */
UCLASS()
class NEXUSBATTLE_API UBTTask_MoveToNexus : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_MoveToNexus();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

