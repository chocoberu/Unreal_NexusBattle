// Fill out your copyright notice in the Description page of Project Settings.


#include "NBMinionAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName ANBMinionAIController::TargetPosKey(TEXT("TargetPos"));
const FName ANBMinionAIController::TargetKey(TEXT("Target"));
const FName ANBMinionAIController::EnemyNexusKey(TEXT("EnemyNexus"));
const FName ANBMinionAIController::EnemyNexusPosKey(TEXT("EnemyNexusPos"));

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
	DetectRange = 300.0f;
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
		GetBlackboardComponent()->SetValueAsObject(ANBMinionAIController::TargetKey, nullptr);
		GetBlackboardComponent()->SetValueAsObject(ANBMinionAIController::TargetPosKey, nullptr);
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}

void ANBMinionAIController::SetAttackInstigator(ACharacter* NewInstigator)
{
	AttackInstigator = NewInstigator;
}

ACharacter* ANBMinionAIController::GetAttackInstigator()
{
	return AttackInstigator;
}
