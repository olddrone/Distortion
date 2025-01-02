// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_Projectile.h"
#include "Components/BoxComponent.h"
#include "ObjectPool/DT_PoolSubSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"

ADT_Projectile::ADT_Projectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bSimulationEnabled = false;
}

void ADT_Projectile::BeginPlay()
{
	Super::BeginPlay();	
	CollisionBox->OnComponentHit.AddUniqueDynamic(this, &ADT_Projectile::OnHit);
}

void ADT_Projectile::OnSpawnFromPool(const FVector_NetQuantize& Location, const FRotator& Rotation)
{
	SetActorEnableCollision(true);

	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	FVector Direction = GetActorForwardVector().GetSafeNormal();
	ProjectileMovementComponent->Velocity = Direction * ProjectileSpeed;

	ProjectileMovementComponent->SetUpdatedComponent(CollisionBox);	// 문제 해결
	ProjectileMovementComponent->bSimulationEnabled = true;
	ProjectileMovementComponent->UpdateComponentVelocity();
	ProjectileMovementComponent->Activate();

	ShowTrace();

	if (GetWorld()->GetTimerManager().IsTimerActive(Handle))
		GetWorld()->GetTimerManager().ClearTimer(Handle);

	GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&] {
		UDT_PoolSubSystem* PoolSubSystem = GetWorld()->GetSubsystem<UDT_PoolSubSystem>();
		PoolSubSystem->ReturnToPool(this);
	}), 1.f, false, -1.0f);
}

void ADT_Projectile::OnReturnToPool()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(Handle))
		GetWorld()->GetTimerManager().ClearTimer(Handle);

	SetActorEnableCollision(false);

	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMovementComponent->bSimulationEnabled = false;
	ProjectileMovementComponent->UpdateComponentVelocity();
	ProjectileMovementComponent->Deactivate();

}

void ADT_Projectile::ShowTrace()
{
	ServerPRCShowTrace();
}

void ADT_Projectile::ServerPRCShowTrace_Implementation()
{
	MulitcastRPCShowTrace();
}

void ADT_Projectile::MulitcastRPCShowTrace_Implementation()
{
	if (TraceParticle)
	{
		TraceComponent = UGameplayStatics::SpawnEmitterAttached(TraceParticle, CollisionBox,
			FName(), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition);
	}
}

void ADT_Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// UE_LOG(LogTemp, Warning, TEXT("This : %s, HitActor : %s"), *GetName(), *OtherActor->GetName());
	
	UDT_PoolSubSystem* PoolSubSystem = GetWorld()->GetSubsystem<UDT_PoolSubSystem>();
	PoolSubSystem->ReturnToPool(this);
}
