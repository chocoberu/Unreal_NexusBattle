// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetTargetOnNexus.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NBBaseAICharacter.h"
#include "NBBaseCharacter.h"
#include "NBMinionAIController.h"
#include "NBNexus.h"

UBTTask_SetTargetOnNexus::UBTTask_SetTargetOnNexus()
{

}

EBTNodeResult::Type UBTTask_SetTargetOnNexus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	auto Nexus = Cast<ANBBaseAICharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ANBMinionAIController::EnemyNexusKey));
	auto ControllingPawn = Cast<ANBBaseAICharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	auto PawnTeam = ControllingPawn->GetMyTeam();

	if (Nexus == nullptr)
	{
		// TODO : Nexus ¼³Á¤
		for (TObjectIterator<ANBNexus> It; It; ++It)
		{
			if (It->GetMyTeam() != PawnTeam && It->GetMyTeam() != ETeam::Neutral)
			{
				Nexus = *It;
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANBMinionAIController::EnemyNexusKey, Nexus);
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(ANBMinionAIController::EnemyNexusPosKey, Nexus->GetActorLocation());
				break;
			}
		}
	}
	else
	{
		return EBTNodeResult::Succeeded;
	}
	Nexus = Cast<ANBBaseAICharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ANBMinionAIController::EnemyNexusKey));

	if (Nexus != nullptr)
	{
		//OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANBMinionAIController::TargetKey, Nexus);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(ANBMinionAIController::EnemyNexusPosKey, Nexus->GetActorLocation());
		NBLOG(Warning, TEXT("Nexus Set"));
		return EBTNodeResult::Succeeded;
	}
	else
		return EBTNodeResult::Failed;
	
}
