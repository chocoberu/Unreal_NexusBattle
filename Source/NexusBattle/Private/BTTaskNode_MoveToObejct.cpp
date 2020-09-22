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

    auto Target = Cast<ACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ANBMinionAIController::TargetKey));
    if (Target == nullptr)
        return EBTNodeResult::Failed;

    // 타겟이 거리 내에 없다면
    if ((Minion->GetActorLocation() - Target->GetActorLocation()).Size() > MinionAI->GetDetectRange())
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANBMinionAIController::TargetKey, nullptr);
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(ANBMinionAIController::TargetPosKey, Minion->GetActorLocation());
        UAIBlueprintHelperLibrary::SimpleMoveToLocation(OwnerComp.GetAIOwner(), Minion->GetActorLocation());
        return EBTNodeResult::Succeeded;
    }

    UAIBlueprintHelperLibrary::SimpleMoveToActor(OwnerComp.GetAIOwner(), Target);

    return EBTNodeResult::Succeeded;
}
