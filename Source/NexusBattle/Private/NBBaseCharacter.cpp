// Fill out your copyright notice in the Description page of Project Settings.

#include "NBBaseCharacter.h"
#include "NBPlayerController.h"
#include "DrawDebugHelpers.h"
#include "NBCharacterStatComponent.h"

// Sets default values
ANBBaseCharacter::ANBBaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<UNBCharacterStatComponent>(TEXT("CHARACTERSTAT"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

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
}

// Called when the game starts or when spawned
void ANBBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	NBController = Cast<ANBPlayerController>(GetController());
	NBCHECK(NBController != nullptr);
}

// Called every frame
void ANBBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
void ANBBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ANBBaseCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ANBBaseCharacter::LeftRight);
}

float ANBBaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	// TODO : 방어력에 따라 데미지 계산

	FinalDamage -= CharacterStat->GetDefence();
	NBLOG(Warning, TEXT("Actor : %s took Damage : %f, CurrentHP : %f"), *GetName(), FinalDamage, CharacterStat->GetCurrentHP());

	CharacterStat->SetDamage(FinalDamage);
	return FinalDamage;
}

void ANBBaseCharacter::UpDown(float NewAxisValue)
{
	DirectionToMove.X = NewAxisValue;
}

void ANBBaseCharacter::LeftRight(float NewAxisValue)
{
	DirectionToMove.Y = NewAxisValue;
}


