// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MinionAttack.h"
#include "NBMinionAIController.h"
#include "NBNormalMinion.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MinionAttack::UBTTask_MinionAttack()
{
	bNotifyTick = false; //
    IsAttacking = false;
}
EBTNodeResult::Type UBTTask_MinionAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Minion = Cast<ANBNormalMinion>(OwnerComp.GetAIOwner()->GetPawn());
	if (Minion == nullptr)
		return EBTNodeResult::Failed;
	auto Target = Cast<ACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ANBMinionAIController::TargetKey));
	if (Target == nullptr)
		return EBTNodeResult::Failed;
	UAIBlueprintHelperLibrary::SimpleMoveToActor(OwnerComp.GetAIOwner(), Target);

	// È¸Àü
	FVector LookVector = Target->GetActorLocation() - Minion->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	Minion->SetActorRotation(FMath::RInterpTo(Minion->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	Minion->NormalAttack();
	IsAttacking = true;
	
	return EBTNodeResult::Succeeded;
}

void UBTTask_MinionAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	/*Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);*/
}
