// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_Gun.h"
#include "Animation/AnimationAsset.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"

ADT_Gun::ADT_Gun()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));

	SetRootComponent(MeshComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void ADT_Gun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADT_Gun::SetFXVisibility(const bool bVisible)
{
	if (FireAnimation)
	{
		USkeletalMeshComponent* SkeletalMesh = Cast<USkeletalMeshComponent>(MeshComponent);
		SkeletalMesh->PlayAnimation(FireAnimation, false);
	}
}

