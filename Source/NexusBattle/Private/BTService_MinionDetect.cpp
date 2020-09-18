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

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
		return;
	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 1000.0f;

	if (World == nullptr)
		return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);

	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2, // BaseCharacter, minion만 체크
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	ACharacter* CurrentTarget = nullptr;
	if (bResult)
	{
		//for (auto& OverlapResult : OverlapResults)
		//{
		//	// 테스트 코드
		//	NBLOG(Warning, TEXT("Actor Name : %s"), *OverlapResult.Actor->GetName());
		//}
		auto MinionAI = Cast<ANBMinionAIController>(OwnerComp.GetAIOwner());
		if (MinionAI->GetAttackInstigator() != nullptr) // 타격한 상대를 향해 공격
		{
			NBLOG(Warning, TEXT("Change Attack Target : %s"), *MinionAI->GetName());
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANBMinionAIController::TargetKey, MinionAI->GetAttackInstigator());
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(ANBMinionAIController::TargetPosKey, MinionAI->GetAttackInstigator()->GetActorLocation());
			CurrentTarget = MinionAI->GetAttackInstigator();

			DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
			DrawDebugPoint(World, CurrentTarget->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
			DrawDebugLine(World, ControllingPawn->GetActorLocation(), CurrentTarget->GetActorLocation(), FColor::Blue, false, 0.2f);

			MinionAI->SetAttackInstigator(nullptr);
			return;
		}
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANBMinionAIController::TargetPosKey, nullptr);
		for (auto& OverlapResult : OverlapResults)
		{
			// TODO : 우선순위 적용
			ACharacter* NBCharacter = Cast<ACharacter>(OverlapResult.GetActor());
			
			if (NBCharacter)
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
				//return;
			}
		}
		NBLOG(Warning, TEXT("Change Attack Target : %s"), *CurrentTarget->GetName());
		DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
		DrawDebugPoint(World, CurrentTarget->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
		DrawDebugLine(World, ControllingPawn->GetActorLocation(), CurrentTarget->GetActorLocation(), FColor::Blue, false, 0.2f);
	}
	else
		DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
