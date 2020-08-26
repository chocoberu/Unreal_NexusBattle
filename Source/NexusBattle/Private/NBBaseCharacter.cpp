// Fill out your copyright notice in the Description page of Project Settings.

#include "NBBaseCharacter.h"

// Sets default values
ANBBaseCharacter::ANBBaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	// ī�޶� ����
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

}

// Called when the game starts or when spawned
void ANBBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ANBBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// InterpTo() : ������ �ӷ����� ��ǥ �������� �����ϵ�, ��ǥ �������� �����ϸ� �� ������ ���ߴ� ���
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

void ANBBaseCharacter::UpDown(float NewAxisValue)
{
	DirectionToMove.X = NewAxisValue;
}

void ANBBaseCharacter::LeftRight(float NewAxisValue)
{
	DirectionToMove.Y = NewAxisValue;
}


