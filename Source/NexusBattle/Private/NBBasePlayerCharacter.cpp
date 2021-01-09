// Fill out your copyright notice in the Description page of Project Settings.


#include "NBBasePlayerCharacter.h"
#include "NBPlayerController.h"
#include "DrawDebugHelpers.h"
#include "NBCharacterStatComponent.h"
#include "Components/WidgetComponent.h"
#include "NBCharacterWidget.h"

// Sets default values
ANBBasePlayerCharacter::ANBBasePlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 서브오브젝트
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<UNBCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	HPBarWidget->SetupAttachment(GetMesh());

	// 카메라 관련
	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f;
	ArmLengthTo = 750.0f;
	ArmRotationTo = FRotator(-45.0f, 0.0f, 0.0f);
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bDoCollisionTest = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	// 콜리전 설정
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NBCharacter"));

	// HP bar 설정
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget>
		UI_HUD(TEXT("/Game/UI/UI_HPBar.UI_HPBar_C"));

	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(200.0f, 50.0f));
	}

}

// Called when the game starts or when spawned
void ANBBasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	NBLOG(Warning, TEXT("base player character beginPlay"));

	auto CharacterWidget = Cast<UNBCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	if (CharacterWidget != nullptr)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
	}

	// TODO : 컨트롤러 관련 오류 해결 
	/*NBController = Cast<ANBPlayerController>(GetController());
	NBCHECK(NBController != nullptr);*/
}

// Called every frame
void ANBBasePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 카메라 위치 조정
	// InterpTo() : 지정한 속력으로 목표 지점까지 진행하되, 목표 지점까지 도달하면 그 값에서 멈추는 기능
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);
	SpringArm->SetRelativeRotation(FMath::RInterpTo(SpringArm->GetRelativeRotation(), ArmRotationTo, DeltaTime, ArmRotationSpeed));
	if (DirectionToMove.SizeSquared() > 0.0f)
	{
		GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
		AddMovementInput(DirectionToMove);
	}
}

// Called to bind functionality to input
void ANBBasePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ANBBasePlayerCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ANBBasePlayerCharacter::LeftRight);
}

void ANBBasePlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

float ANBBasePlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	FinalDamage -= CharacterStat->GetDefence();

	CharacterStat->SetDamage(FinalDamage);
	NBLOG(Warning, TEXT("Actor : %s took Damage : %f, CurrentHP : %f"), *GetName(), FinalDamage, CharacterStat->GetCurrentHP());
	return FinalDamage;
}

void ANBBasePlayerCharacter::OnDead()
{
	Super::OnDead();

}

void ANBBasePlayerCharacter::UpDown(float NewAxisValue)
{
	DirectionToMove.X = NewAxisValue;
}

void ANBBasePlayerCharacter::LeftRight(float NewAxisValue)
{
	DirectionToMove.Y = NewAxisValue;
}

void ANBBasePlayerCharacter::AddExp(float NewExp)
{
	CharacterStat->AddExp(NewExp);
}