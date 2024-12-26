// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_Gun.h"
#include "Animation/AnimationAsset.h"

ADT_Gun::ADT_Gun()
{
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(MeshComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void ADT_Gun::Attack(const FDamagePacket& DamagePacket)
{
	if (FireAnimation)
	{
		USkeletalMeshComponent* SkeletalMesh = Cast<USkeletalMeshComponent>(MeshComponent);
		SkeletalMesh->PlayAnimation(FireAnimation, false);
	}
}
