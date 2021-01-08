// Fill out your copyright notice in the Description page of Project Settings.


#include "NBTurretProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ANBTurretProjectile::ANBTurretProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
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

	RootComponent = CollisionComponent;
	ProjectileParticleSystem->SetupAttachment(RootComponent);

	// 구체의 콜리전 반경을 설정합니다.
	CollisionComponent->InitSphereRadius(20.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	//CollisionComponent->OnComponentHit.AddDynamic(this, &ANBTurretProjectile::OnHit);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ANBTurretProjectile::OnOverlapBegin);
	ProjectileParticleSystem->SetRelativeLocation(FVector(-80.0f, 0.0f, 0.0f));
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

	if (TargetActor != nullptr && ProjectileParticleSystem->IsActive())
	{
		FVector Dir = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		SetActorLocation(GetActorLocation() + Dir * DeltaTime * Speed);
		SetActorRotation(Dir.Rotation());

	}
}
void ANBTurretProjectile::OnOverlapBegin(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (OtherActor != TargetActor)
		return;
	NBLOG(Warning, TEXT("OnHit() Call"));
	// 총알의 속도를 0으로 설정
	Speed = 0.0f;
	SetActorEnableCollision(false); // 충돌 비활성화
	SetHitParticle();

	// TODO : 데미지 적용
	
}
void ANBTurretProjectile::ShotToTarget(class AActor* Target)
{
	if (Target == nullptr)
		return;
	TargetActor = Target;
	ProjectileParticleSystem->Activate(true);
}
void ANBTurretProjectile::SetHitParticle()
{
	if(HitParticle != nullptr)
		ProjectileParticleSystem->SetTemplate(HitParticle);
	SetLifeSpan(1.0f);
}
