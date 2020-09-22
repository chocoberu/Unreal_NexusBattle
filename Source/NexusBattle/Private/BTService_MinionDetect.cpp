// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_MinionDetect.h"
#include "NBMinionAIController.h"
#include "NBNormalMinion.h"
#include "NBBaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_MinionDetect::UBTService_MinionDetect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_MinionDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ANBBaseCharacter* ControllingPawn = Cast<ANBBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
		return;
	
	auto MinionAI = Cast<ANBMinionAIController>(OwnerComp.GetAIOwner());
	if (MinionAI == nullptr)
		return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = MinionAI->GetDetectRange();

	if (World == nullptr)
		return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);

	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2, // BaseCharacter, minion, nexus, turret만 체크
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	ANBBaseCharacter* CurrentTarget = nullptr;
	if (bResult)
	{
		if (MinionAI->GetAttackInstigator() != nullptr) // 타격한 상대를 향해 공격
		{
			auto AttackInstigator = Cast<ANBBaseCharacter>(MinionAI->GetAttackInstigator());
			if (ControllingPawn->GetMyTeam() != AttackInstigator->GetMyTeam())
			{
				NBLOG(Warning, TEXT("Change Attack Target : %s"), *MinionAI->GetName());
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANBMinionAIController::TargetKey, MinionAI->GetAttackInstigator());
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(ANBMinionAIController::TargetPosKey, MinionAI->GetAttackInstigator()->GetActorLocation());
				CurrentTarget = AttackInstigator;

				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				DrawDebugPoint(World, CurrentTarget->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), CurrentTarget->GetActorLocation(), FColor::Blue, false, 0.2f);

				MinionAI->SetAttackInstigator(nullptr);
				return;
			}
		}
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANBMinionAIController::TargetPosKey, nullptr);
		for (auto& OverlapResult : OverlapResults)
		{
			// TODO : 우선순위 적용
			// 1순위 : 미니언, 2순위 : 소환 몬스터, 3순위: 터렛, 4순위 : 캐릭터
			ANBBaseCharacter* NBCharacter = Cast<ANBBaseCharacter>(OverlapResult.GetActor());
			
			if (NBCharacter && NBCharacter->GetMyTeam() != ControllingPawn->GetMyTeam())
			{
				if (CurrentTarget == nullptr)
				{
					CurrentTarget = NBCharacter;
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANBMinionAIController::TargetKey, NBCharacter);
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(ANBMinionAIController::TargetPosKey, NBCharacter->GetActorLocation());
					continue;
				}
				if ((NBCharacter->GetActorLocation() - Center).Size() < (CurrentTarget->GetActorLocation() - Center).Size())
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANBMinionAIController::TargetKey, NBCharacter);
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(ANBMinionAIController::TargetPosKey, NBCharacter->GetActorLocation());
					CurrentTarget = NBCharacter;
				}
			}
		}	
	}
	// 현재 타겟이 죽은 경우
	if (CurrentTarget != nullptr && CurrentTarget->GetCharacterState() == ECharacterState::Dead)
	{
		NBLOG(Warning, TEXT("Enemy Dead!"));
		CurrentTarget = nullptr;
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANBMinionAIController::TargetKey, nullptr);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANBMinionAIController::TargetPosKey, nullptr);
	}
	// 현재 타겟이 정해졌다면
	if (CurrentTarget != nullptr)
	{
		//NBLOG(Warning, TEXT("Change Attack Target : %s"), *CurrentTarget->GetName());
		NBLOG(Warning, TEXT("Target : %s, State : %d"), *CurrentTarget->GetName(), CurrentTarget->GetCharacterState());
		DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
		DrawDebugPoint(World, CurrentTarget->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
		DrawDebugLine(World, ControllingPawn->GetActorLocation(), CurrentTarget->GetActorLocation(), FColor::Blue, false, 0.2f);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANBMinionAIController::TargetKey, nullptr);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANBMinionAIController::TargetPosKey, nullptr);
		DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
	}
}
