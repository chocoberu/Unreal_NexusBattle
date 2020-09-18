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


	if (bResult)
	{
		for (auto& OverlapResult : OverlapResults)
		{
			// 테스트 코드
			NBLOG(Warning, TEXT("Actor Name : %s"), *OverlapResult.Actor->GetName());
		}
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANBMinionAIController::TargetPosKey, nullptr);
		for (auto& OverlapResult : OverlapResults)
		{
			ANBBaseCharacter* NBCharacter = Cast<ANBBaseCharacter>(OverlapResult.GetActor());
			// TODO : 우선순위 적용
			if (NBCharacter)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANBMinionAIController::TargetKey, NBCharacter);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				DrawDebugPoint(World, NBCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), NBCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
				return;
			}
		}
	}
	else
		DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
