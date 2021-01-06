// Fill out your copyright notice in the Description page of Project Settings.


#include "NBGunnerCharacter.h"
#include "NBGunnerAnimInstance.h"
#include "DrawDebugHelpers.h"
#include "NBCharacterStatComponent.h"
#include "Components/WidgetComponent.h"
#include "NBCharacterWidget.h"
#include "NBPlayerController.h"

ANBGunnerCharacter::ANBGunnerCharacter()
{
	// 컴포넌트 생성
	
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_GUNNER(TEXT("/Game/ParagonTwinblast/Characters/Heroes/TwinBlast/Meshes/TwinBlast.TwinBlast"));

	if (SK_GUNNER.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_GUNNER.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	// 애님 인스턴스
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		GUNNER_ANIM(TEXT("/Game/ParagonTwinblast/Characters/Heroes/TwinBlast/GunnerAnimBlueprint.GunnerAnimBlueprint_C"));

	if (GUNNER_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(GUNNER_ANIM.Class);
	}

	IsAttacking = false;

	// 공격 범위
	AttackRange = 300.0f;
	AttackRadius = 50.0f;
}
void ANBGunnerCharacter::BeginPlay()
{
	Super::BeginPlay();

	NBLOG(Warning, TEXT("gunner character beginPlay"));

	// 캐릭터 무브먼트로 속도 테스트
	GetCharacterMovement()->MaxWalkSpeed = 700.0f;
	GetCharacterMovement()->JumpZVelocity = 200.0f;

}

void ANBGunnerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 현재 캐릭터 속도 log
	//NBLOG(Warning, TEXT("Current Speed : %f"), GetCharacterMovement()->Velocity.Size());
}

void ANBGunnerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("NormalAttack"), EInputEvent::IE_Pressed, this, &ANBGunnerCharacter::NormalAttack);
	PlayerInputComponent->BindAction(TEXT("Skill1"), EInputEvent::IE_Pressed, this, &ANBGunnerCharacter::RocketDash);
}

void ANBGunnerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GunnerAnim = Cast<UNBGunnerAnimInstance>(GetMesh()->GetAnimInstance());
	NBCHECK(GunnerAnim != nullptr);

	GunnerAnim->OnMontageEnded.AddDynamic(this, &ANBGunnerCharacter::OnAttackMontageEnded);
	GunnerAnim->OnAttackHitCheck.AddUObject(this, &ANBGunnerCharacter::NormalAttackCheck);

	CharacterStat->OnHPIsZero.AddLambda([this]() -> void
		{
			NBLOG(Warning, TEXT("Gunner HP is zero"));
			SetCharacterState(ECharacterState::Dead);
			GunnerAnim->SetDeadAnim();
			SetActorEnableCollision(false);
			HPBarWidget->SetHiddenInGame(true);
		});
}

void ANBGunnerCharacter::NormalAttack()
{
	if (IsAttacking)
		return;

	// TODO
	GunnerAnim->PlayNormalAttackMontage();

	IsAttacking = true;
}
void ANBGunnerCharacter::RocketDash()
{
	// TODO : 모든 방향 적용 필요
	
	// TODO : 점프 개선(순간 속도 등)

	//NBLOG(Warning, TEXT("Call RocketDash"));

	// 테스트 코드
	float Velocity = 900.0f; // 순간 속도
	GetCharacterMovement()->Velocity = GetActorForwardVector() * Velocity;
	Jump();
	GunnerAnim->PlayRocketDashFrontMontage();

}

void ANBGunnerCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage != GunnerAnim->GetNormalAttackMontage())
	{
		//NBLOG(Warning, TEXT("Not Normal Attack"));
		return;
	}
	NBCHECK(IsAttacking);
	IsAttacking = false;
}

void ANBGunnerCharacter::NormalAttackCheck()
{
	NBLOG(Warning, TEXT("NormalAttack Check"));
	FHitResult HitResult;
	
	FCollisionQueryParams Params(NAME_None, false, this);

	for (TObjectIterator<ANBBaseCharacter> It; It; ++It)
	{
		if ((*It)->GetMyTeam() == MyTeam)
		{
			Params.AddIgnoredActor(*It);
			//NBLOG(Warning, TEXT("Same Team Actor : %s"), *(*It)->GetName());
		}
	}
	
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(), // 시작점
		GetActorLocation() + GetActorForwardVector() * AttackRange, // 끝점
		FQuat::Identity, // 회전값
		ECollisionChannel::ECC_GameTraceChannel2, // 트레이스채널
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

#if ENABLE_DRAW_DEBUG

	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 1.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);
#endif
	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			NBLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());
			auto NBBaseCharacter = Cast<ANBBaseCharacter>(HitResult.Actor.Get());
			if (NBBaseCharacter == nullptr)
				return;
			if (NBBaseCharacter->GetMyTeam() == MyTeam && MyTeam != ETeam::Neutral)
				return;

			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(CharacterStat->GetAttack(), // 데미지 크기
				DamageEvent, // 데미지 종류
				GetController(), // 가해자 (컨트롤러)
				this); // 데미지 전달을 위해 사용한 도구 (액터)
			// TODO : 타격 파티클 추가
		}
		/*NBLOG(Warning, TEXT("Total Hit Actors : %d"), HitResults.Num());
		for (auto& Result : HitResults)
		{
			if (Result.Actor.IsValid())
			{
				NBLOG(Warning, TEXT("Hit Actor Name : %s"), *Result.Actor->GetName());
			}
		}*/
	}
}
