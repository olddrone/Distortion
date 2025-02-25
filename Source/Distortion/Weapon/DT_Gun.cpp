// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_Gun.h"
#include "UObject/ConstructorHelpers.h"
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
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Rifle_Ref(
		TEXT("/Game/Blueprints/Weapon/AM_FireRifle.AM_FireRifle"));
	if (Rifle_Ref.Succeeded())
		FireAnimation = Rifle_Ref.Object;

	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Reload_Ref(
		TEXT("/Game/Blueprints/Player/Animations/AM_Reload.AM_Reload"));
	if (Reload_Ref.Succeeded())
		ReloadMontage = Reload_Ref.Object;
	
	InitCrosshairsTexture();
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

void ADT_Gun::InitCrosshairsTexture()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> Center_Ref(
		TEXT("/Game/Assets/Texture/Crosshair_Center.Crosshair_Center"));
	if (Center_Ref.Succeeded())
		Crosshairs.Center = Center_Ref.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D> Top_Ref(
		TEXT("/Game/Assets/Texture/Crosshair_Top.Crosshair_Top"));
	if (Top_Ref.Succeeded())
		Crosshairs.Top = Top_Ref.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D> Bottom_Ref(
		TEXT("/Game/Assets/Texture/Crosshair_Bottom.Crosshair_Bottom"));
	if (Bottom_Ref.Succeeded())
		Crosshairs.Bottom = Bottom_Ref.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D> Left_Ref(
		TEXT("/Game/Assets/Texture/Crosshair_Left.Crosshair_Left"));
	if (Left_Ref.Succeeded())
		Crosshairs.Left = Left_Ref.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D> Right_Ref(
		TEXT("/Game/Assets/Texture/Crosshair_Right.Crosshair_Right"));
	if (Right_Ref.Succeeded())
		Crosshairs.Right = Right_Ref.Object;
}
