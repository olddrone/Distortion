// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_BaseWeapon.h"
#include "GameFramework/Character.h"

ADT_BaseWeapon::ADT_BaseWeapon()
{
}

void ADT_BaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector ADT_BaseWeapon::GetSocketLocation(const FName& SocketName) const
{
	return GetMeshComp()->GetSocketLocation(SocketName);
}

void ADT_BaseWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	if (IsValid(MeshComponent))
	{
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		MeshComponent->AttachToComponent(InParent, TransformRules, InSocketName);
	}
}

void ADT_BaseWeapon::Equip(APawn* OwnerPawn, const FName& InSocketName, UDT_CollisionManager* InCollisionManager)
{
	SetOwner(OwnerPawn);
	SetInstigator(OwnerPawn);
	AttachMeshToSocket(Cast<ACharacter>(OwnerPawn)->GetMesh(), InSocketName);
}
