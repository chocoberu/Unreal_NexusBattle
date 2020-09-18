// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NexusBattle.h"
#include "BehaviorTree/BTService.h"
#include "BTService_MinionDetect.generated.h"

/**
 * 
 */
UCLASS()
class NEXUSBATTLE_API UBTService_MinionDetect : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_MinionDetect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
