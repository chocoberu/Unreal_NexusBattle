// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToNexus.h"
#include "NBMinionAIController.h"
#include "NBNormalMinion.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MoveToNexus::UBTTask_MoveToNexus()
{
	NodeName = TEXT("MoveToNexus");
}
EBTNodeResult::Type UBTTask_MoveToNexus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    auto Minion = Cast<ANBNormalMinion>(OwnerComp.GetAIOwner()->GetPawn());
    if (Minion == nullptr)
        return EBTNodeResult::Failed;

    auto MinionAI = Cast<ANBMinionAIController>(OwnerComp.GetAIOwner());
    if (MinionAI == nullptr)
        return EBTNodeResult::Failed;

    // 테스트 코드
    //NBLOG(Warning, TEXT("Minion HP Ratio : %f"), Minion->GetHPRatio());
    if (Minion->GetHPRatio() <= 0.0f)
    {
        NBLOG(Warning, TEXT("Dead Minion"));
        return EBTNodeResult::Failed;;
    }

    auto Target = Cast<ACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ANBMinionAIController::EnemyNexusKey));
    if (Target == nullptr)
        return EBTNodeResult::Failed;

    UAIBlueprintHelperLibrary::SimpleMoveToActor(OwnerComp.GetAIOwner(), Target);

    return EBTNodeResult::Succeeded;
}
