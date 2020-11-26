// Fill out your copyright notice in the Description page of Project Settings.


#include "NBNormalMinion.h"
#include "NBMinionAnimInstance.h"
#include "Components/WidgetComponent.h"
#include "NBMinionWidget.h"
#include "NBMinionAIController.h"
#include "DrawDebugHelpers.h"

// Sets default values
ANBNormalMinion::ANBNormalMinion()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	HPBarWidget->SetupAttachment(GetMesh());*/

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_MINION(TEXT("/Game/ParagonMinions/Characters/Minions/Down_Minions/Meshes/Minion_Lane_Melee_Dawn.Minion_Lane_Melee_Dawn"));

	if (SK_MINION.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_MINION.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance>
		MINION_ANIM(TEXT("/Game/Animation/NormalMinionAnimBlueprint.NormalMinionAnimBlueprint_C"));

	if (MINION_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(MINION_ANIM.Class);
	}
	// 콜리전 설정
	//GetCapsuleComponent()->SetCollisionProfileName(TEXT("NBCharacter"));

	// HP Bar
	/*HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);*/

	/*static ConstructorHelpers::FClassFinder<UUserWidget>
		UI_HUD(TEXT("/Game/UI/UI_HPBar_Minion.UI_HPBar_Minion_C"));

	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}*/

	// HP
	CurrentHP = 100.0f;

	AttackRange = 170.0f;
	AttackRadius = 50.0f;
	IsAttacking = false;
	MaxCombo = 4;
	AttackEndComboState();

	AIControllerClass = ANBMinionAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}

// Called when the game starts or when spawned
void ANBNormalMinion::BeginPlay()
{
	Super::BeginPlay();
	
	MinionAIController = Cast<ANBMinionAIController>(GetController());
}

// Called every frame
void ANBNormalMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

// Called to bind functionality to input
void ANBNormalMinion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANBNormalMinion::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MinionAnim = Cast<UNBMinionAnimInstance>(GetMesh()->GetAnimInstance());

	MinionAnim->OnMontageEnded.AddDynamic(this, &ANBNormalMinion::OnAttackMontageEnded);
	MinionAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		
		NBLOG(Warning, TEXT("Minion OnNextAttackCheck"));
		
		
		// TODO : 타겟이 존재하면 계속 공격하도록 변경
		CanNextCombo = true;

		if (IsComboInputOn)
		{
			AttackStartComboState();
			MinionAnim->JumpToAttackMontageSection(CurrentCombo);
		}
		});
	MinionAnim->OnAttackHitCheck.AddUObject(this, &ANBNormalMinion::NormalAttackCheck);
}

float ANBNormalMinion::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CurrentHP = FMath::Clamp<float>(CurrentHP - FinalDamage, 0.0f, 100.0f);

	NBLOG(Warning, TEXT("Minion Current HP : %f"), CurrentHP);
	
	if (MinionAIController != nullptr)
	{
		// 타격한 상대를 타겟으로
		MinionAIController->SetAttackInstigator(EventInstigator->GetCharacter());
	}
	OnHPChanged.Broadcast();
	// 테스트 코드
	// TODO : hpbar, 공격 추가, 팀
	if (CurrentHP <= 0.0f)
	{
		//if (EventInstigator->IsPlayerController())
		//{
		//	// TODO : 경험치 관련
		//}
		SetCharacterState(ECharacterState::Dead);
		MinionAnim->SetDeadAnim();
		MinionAIController->StopAI();
		SetActorEnableCollision(false);
		HPBarWidget->SetHiddenInGame(true);
	}
	return FinalDamage;
}

void ANBNormalMinion::NormalAttack()
{
	if (IsAttacking)
	{
		NBCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (CanNextCombo)
			IsComboInputOn = true;
	}
	else
	{
		NBCHECK(CurrentCombo == 0);
		AttackStartComboState();
		MinionAnim->PlayNormalAttackMontage();
		MinionAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}
}

void ANBNormalMinion::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	NBCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void ANBNormalMinion::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void ANBNormalMinion::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	NBCHECK(IsAttacking);
	NBCHECK(CurrentCombo > 0);
	IsAttacking = false;
	AttackEndComboState();
}

void ANBNormalMinion::NormalAttackCheck()
{
	// TODO : 공격 체크 추가

	NBLOG(Warning, TEXT("Minion NormalAttack Check"));
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	for (TObjectIterator<ANBBaseCharacter> It; It; ++It)
	{
		if ((*It)->GetMyTeam() == MyTeam)
		{
			Params.AddIgnoredActor(*It);
			// NBLOG(Warning, TEXT("Same Team Actor : %s"), *(*It)->GetName());
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
	FColor DrawColor = bResult ? FColor::Blue : FColor::Red;
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
			HitResult.Actor->TakeDamage(10.0f, // 데미지 크기
				DamageEvent, // 데미지 종류
				GetController(), // 가해자 (컨트롤러)
				this); // 데미지 전달을 위해 사용한 도구 (액터)
			// TODO : 타격 파티클 추가
		}
	}
}
