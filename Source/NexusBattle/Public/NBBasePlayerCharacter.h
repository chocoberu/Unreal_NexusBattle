// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NexusBattle.h"
#include "NBBaseCharacter.h"
#include "NBBasePlayerCharacter.generated.h"

UCLASS()
class NEXUSBATTLE_API ANBBasePlayerCharacter : public ANBBaseCharacter
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ANBBasePlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	ETeam GetMyTeam() { return MyTeam; }

	// ����, ��ų ���� �Լ�

	// ����ġ �߰�
	UFUNCTION() void AddExp(float NewExp);

	virtual void OnDead();
protected:
	// �̵� ���� ���� ����
	FVector DirectionToMove = FVector::ZeroVector;

	// ������ �� ���� ����
	float ArmLengthTo = 0.0f;
	FRotator ArmRotationTo = FRotator::ZeroRotator;
	float ArmLengthSpeed = 0.0f;
	float ArmRotationSpeed = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = Camera) USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = Camera) UCameraComponent* Camera;

	UPROPERTY() class ANBPlayerController* NBController; // �÷��̾� ��Ʈ�ѷ�

	// �̵� ���� �Լ�
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true)) float AttackRange;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true)) float AttackRadius;

	UPROPERTY(VisibleAnywhere, Category = Stat) class UNBCharacterStatComponent* CharacterStat;
	UPROPERTY(VisibleAnywhere, Category = UI) class UWidgetComponent* HPBarWidget;

};
