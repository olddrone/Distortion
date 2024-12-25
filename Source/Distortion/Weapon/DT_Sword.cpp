// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_Sword.h"
#include "Component/DT_CollisionManager.h"
#include "Component/DT_CombatComponent.h"

ADT_Sword::ADT_Sword()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(MeshComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void ADT_Sword::BeginPlay()
{
	Super::BeginPlay();
}

void ADT_Sword::Attack(const FDamagePacket& DamagePacket)
{
	if (IsValid(CollisionManager))
	{
		CollisionManager->SetSocketName(DamagePacket.StartSocketName, DamagePacket.EndSocketName);
		CollisionManager->DoCollision(this);
	}
}

void ADT_Sword::AttackEnd()
{
	if (IsValid(CollisionManager))
		CollisionManager->StopCollision();
}

void ADT_Sword::Equip(APawn* OwnerPawn, const FName& InSocketName, UDT_CollisionManager* InCollisionManager)
{
	Super::Equip(OwnerPawn, InSocketName, InCollisionManager);

	CollisionManager = InCollisionManager;
	if (Cast<APawn>(OwnerPawn)->IsLocallyControlled())
	{
		CollisionManager->SetCharacter(OwnerPawn);
		CollisionManager->SetActorsToIgnore(OwnerPawn);
		CollisionManager->SetActorsToIgnore(this);
	}
}

