// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_Projectile.h"
#include "Components/BoxComponent.h"

ADT_Projectile::ADT_Projectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
}

void ADT_Projectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADT_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADT_Projectile::OnSpawnFromPool_Implementation()
{
}

void ADT_Projectile::OnReturnToPool_Implementation()
{
}
