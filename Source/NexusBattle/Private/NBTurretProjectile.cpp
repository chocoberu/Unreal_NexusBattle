// Fill out your copyright notice in the Description page of Project Settings.


#include "NBTurretProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ANBTurretProjectile::ANBTurretProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	ProjectileParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ProjectileParticle"));
	
	// 파티클 시스템
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		PT_PROJECTILE_PARTICLE(TEXT("/Game/ParagonProps/FX/Particles/Turret/P_SentryTurretProjectile.P_SentryTurretProjectile"));

	if (PT_PROJECTILE_PARTICLE.Succeeded())
	{
		ProjectileParticleSystem->SetTemplate(PT_PROJECTILE_PARTICLE.Object);
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		PT_HIT_PARTICLE(TEXT("/Game/ParagonProps/FX/Particles/Turret/P_SentryTurret_HeroImpact_Proj.P_SentryTurret_HeroImpact_Proj"));

	if (PT_HIT_PARTICLE.Succeeded())
	{
		HitParticle = PT_HIT_PARTICLE.Object;
	}

	RootComponent = ProjectileParticleSystem;
	CollisionComponent->SetupAttachment(RootComponent);

	// 구체의 콜리전 반경을 설정합니다.
	CollisionComponent->InitCapsuleSize(20.0f, 50.0f);
	CollisionComponent->SetRelativeLocationAndRotation(FVector(80.0f, 0.0f, 0.0f), FRotator(90.0f, 0.0f, 0.0f));
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->SetNotifyRigidBodyCollision(true);
	CollisionComponent->OnComponentHit.AddDynamic(this, &ANBTurretProjectile::OnHit);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ANBTurretProjectile::OnOverlapBegin);
	/*ProjectileParticleSystem->SetNotifyRigidBodyCollision(true);
	ProjectileParticleSystem->SetCollisionProfileName(TEXT("Projectile"));
	ProjectileParticleSystem->OnComponentHit.AddDynamic(this, &ANBTurretProjectile::OnHit);*/

	Speed = 1000.0f;
}

// Called when the game starts or when spawned
void ANBTurretProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileParticleSystem->Activate(false);
}

// Called every frame
void ANBTurretProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TargetActor != nullptr && TargetActor->GetActorEnableCollision() == false)
	{
		Destroy();
	}

	if (TargetActor != nullptr && ProjectileParticleSystem->IsActive())
	{
		FVector Dir = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		SetActorLocation(GetActorLocation() + Dir * DeltaTime * Speed);
		SetActorRotation(Dir.Rotation());

	}
}
void ANBTurretProjectile::OnOverlapBegin(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (TargetActor != OtherActor)
		return;
	NBLOG(Warning, TEXT("OnOverlapBegin() Call : Hit Actor : %s"), *OtherActor->GetName());
	// 총알의 속도를 0으로 설정
	Speed = 0.0f;
	SetActorEnableCollision(false); // 충돌 비활성화
	SetHitParticle();

	// TODO : 데미지 적용
	FDamageEvent DamageEvent;
	OtherActor->TakeDamage(20.0f, DamageEvent, nullptr, this);
}
void ANBTurretProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	NBLOG(Warning, TEXT("OnHit() Call"));

	Speed = 0.0f;
	SetHitParticle();
}
void ANBTurretProjectile::ShotToTarget(class AActor* Target, AController* Controller)
{
	if (Target == nullptr)
		return;
	TargetActor = Target;
	TargetController = Controller;
	ProjectileParticleSystem->Activate(true);
}
void ANBTurretProjectile::SetHitParticle()
{
	if(HitParticle != nullptr)
		ProjectileParticleSystem->SetTemplate(HitParticle);
	SetActorEnableCollision(false); // 충돌 비활성화
	SetLifeSpan(1.0f);
}
