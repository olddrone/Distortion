// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_Gun.h"
#include "Animation/AnimationAsset.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"

ADT_Gun::ADT_Gun()
{
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));

	SetRootComponent(MeshComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void ADT_Gun::Attack(const FDamagePacket& DamagePacket, const FVector_NetQuantize& TraceHitTarget)
{
	if (!CanFire())
		return;
	--Ammo;
}

void ADT_Gun::SetFXVisibility(const bool bVisible)
{
	if (FireAnimation)
	{
		USkeletalMeshComponent* SkeletalMesh = Cast<USkeletalMeshComponent>(MeshComponent);
		SkeletalMesh->PlayAnimation(FireAnimation, false);
	}
}

