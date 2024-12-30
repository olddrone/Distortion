// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_Projectile.h"
#include "Components/BoxComponent.h"
#include "ObjectPool/DT_PoolSubSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"

ADT_Projectile::ADT_Projectile()
{
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

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

	ProjectileMovementComponent->bSimulationEnabled = true;
	FVector Direction = GetActorForwardVector().GetSafeNormal();
	ProjectileMovementComponent->Velocity = Direction * ProjectileSpeed;
	ProjectileMovementComponent->Activate();

}

void ADT_Projectile::OnReturnToPool()
{
	SetActorEnableCollision(false);

	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovementComponent->bSimulationEnabled = false;
	ProjectileMovementComponent->Deactivate();

}

void ADT_Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());

	UDT_PoolSubSystem* PoolSubSystem = GetWorld()->GetSubsystem<UDT_PoolSubSystem>();
	PoolSubSystem->ReturnToPool(this);
}
