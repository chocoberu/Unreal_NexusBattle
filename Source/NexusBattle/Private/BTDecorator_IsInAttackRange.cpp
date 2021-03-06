// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange.h"
#include "NBMinionAIController.h"
#include "NBNormalMinion.h"
#include "NBBaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
    NodeName = TEXT("CanAttack");
}
bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (ControllingPawn == nullptr)
        return false;

    auto MinionAI = Cast<ANBMinionAIController>(OwnerComp.GetAIOwner());
    if (MinionAI == nullptr)
        return false;

    auto Target = Cast<ANBBaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ANBMinionAIController::TargetKey));
    if (Target == nullptr)
    {
        NBLOG(Warning, TEXT("No Target"));
        return false;
    }
    bResult = (Target->GetDistanceTo(ControllingPawn) <= MinionAI->GetDetectRange());
        return bResult;
}
