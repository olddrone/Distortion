// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_Gun.h"
#include "Animation/AnimationAsset.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

ADT_Gun::ADT_Gun() : Ammo(MaxAmmo)
{
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));

	SetRootComponent(MeshComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void ADT_Gun::Equip(APawn* OwnerPawn, const FName& InSocketName, UDT_CollisionManager* InCollisionManager)
{
	Super::Equip(OwnerPawn, InSocketName, InCollisionManager);
}

void ADT_Gun::SetFXVisibility(const bool bVisible)
{
	if (FireAnimation && bVisible)
	{
		USkeletalMeshComponent* SkeletalMesh = Cast<USkeletalMeshComponent>(MeshComponent);
		SkeletalMesh->PlayAnimation(FireAnimation, false);
	}
}

void ADT_Gun::SetAmmo(const uint8 InAmmo)
{
	Ammo = FMath::Clamp(Ammo - 1, 0, MaxAmmo);
	AmmoChange.ExecuteIfBound(Ammo);
}

void ADT_Gun::SetUI(const bool bIsEquip, IDT_HUDInterface* Interface)
{
	Super::SetUI(bIsEquip, Interface);
	AmmoChangeEvent();

	ESlateVisibility Visible = (bIsEquip) ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	AmmoVisible.ExecuteIfBound(Visible);
}
