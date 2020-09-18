// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_MoveToObejct.h"
#include "NBMinionAIController.h"
#include "NBNormalMinion.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_MoveToObejct::UBTTaskNode_MoveToObejct()
{
    NodeName = TEXT("MoveToObject");
}
EBTNodeResult::Type UBTTaskNode_MoveToObejct::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    auto Minion = Cast<ANBNormalMinion>(OwnerComp.GetAIOwner()->GetPawn());
    if (Minion == nullptr)
        return EBTNodeResult::Failed;

    auto Target = Cast<ACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ANBMinionAIController::TargetKey));
    if (Target == nullptr)
        return EBTNodeResult::Failed;

    UAIBlueprintHelperLibrary::SimpleMoveToActor(OwnerComp.GetAIOwner(), Target);

    return EBTNodeResult::Succeeded;
}
