// Fill out your copyright notice in the Description page of Project Settings.


#include "NBMinionAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

ANBMinionAIController::ANBMinionAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData>
		BBObject(TEXT("/Game/AI/BB_NBMinion.BB_NBMinion"));

	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>
		BTObject(TEXT("/Game/AI/BT_NBMinion.BT_NBMinion"));

	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void ANBMinionAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	RunAI();
}

void ANBMinionAIController::RunAI()
{
	if (UseBlackboard(BBAsset, Blackboard))
	{
		if (!RunBehaviorTree(BTAsset))
		{
			NBLOG(Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}

void ANBMinionAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent != nullptr)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}
