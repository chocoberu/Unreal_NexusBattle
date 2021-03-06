// Fill out your copyright notice in the Description page of Project Settings.


#include "NBNormalMinion.h"
#include "NBMinionAnimInstance.h"
#include "Components/WidgetComponent.h"
#include "NBMinionWidget.h"
#include "NBMinionAIController.h"
#include "NBBasePlayerCharacter.h"
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
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_BLUE_MINION(TEXT("/Game/ParagonMinions/Characters/Minions/Dusk_Minions/Meshes/Minion_Lane_Melee_Dusk.Minion_Lane_Melee_Dusk"));
	if (SK_BLUE_MINION.Succeeded())
	{
		BlueMesh = SK_BLUE_MINION.Object;
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
	CurrentHP = MaxHP = 100.0f;

	// 공격 관련 초기화
	AttackRange = 170.0f;
	AttackRadius = 50.0f;
	IsAttacking = false;
	MaxCombo = 4;
	AttackEndComboState();

	// AI 관련 초기화
	AIControllerClass = ANBMinionAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 미니언 이동 속도
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	// Drop Exp
	DropExp = 10.0f;
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

	if (MinionAIController != nullptr && EventInstigator != nullptr)
	{
		// 타격한 상대를 타겟으로
		MinionAIController->SetAttackInstigator(EventInstigator->GetCharacter());
	}
	
	// 테스트 코드
	// TODO : hpbar, 공격 추가, 팀
	if (CurrentHP <= 0.0f)
	{
		OnDead();
	}
	return FinalDamage;
}

void ANBNormalMinion::OnDead()
{
	Super::OnDead();
	MinionAnim->SetDeadAnim();
	MinionAIController->StopAI();
	
	// TODO : 미니언 삭제 (오브젝트 풀링 적용)
	SetLifeSpan(3.0f);

}
void ANBNormalMinion::SetMyTeam(ETeam NewTeam)
{
	Super::SetMyTeam(NewTeam);

	if (NewTeam == ETeam::Blue && BlueMesh != NULL)
	{
		GetMesh()->SetSkeletalMesh(BlueMesh);
	}
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

	// 같은 팀 캐릭터는 공격 체크에서 제외
	for (TObjectIterator<ANBBaseCharacter> It; It; ++It)
	{
		if ((*It)->GetMyTeam() == MyTeam)
		{
			Params.AddIgnoredActor(*It);
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
