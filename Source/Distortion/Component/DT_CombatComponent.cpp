// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_CombatComponent.h"
#include "Component/DT_CollisionManager.h"
#include "Weapon/DT_BaseWeapon.h"
#include "Library/DT_CustomLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Data/DT_Crosshairs.h"

#include "UI/HUD/DT_HUD.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Interface/DT_GunInterface.h"

UDT_CombatComponent::UDT_CombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> BaseAttackRef(
		TEXT("/Game/Blueprints/Player/Animations/AM_Fist.AM_Fist"));
	if (BaseAttackRef.Object)
		BaseAttackMontage = BaseAttackRef.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeRef(
		TEXT("/Game/Blueprints/Player/Animations/AM_Dodge.AM_Dodge"));
	if (DodgeRef.Object)
		DodgeMontage = DodgeRef.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitRef(
		TEXT("/Game/Blueprints/Player/Animations/AM_Hit.AM_Hit"));
	if (HitRef.Object)
		HitMontage = HitRef.Object;

	CollisionManager = CreateDefaultSubobject<UDT_CollisionManager>(TEXT("CollisionManager"));
}

void UDT_CombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetEquipWeapon())
		SetHUDCrosshairs(DeltaTime);
}

void UDT_CombatComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (IsValid(Owner))
	{
		CombatInterface = Cast<IDT_CombatInterface>(Owner);
		MeshInterface = Cast<IDT_MeshInterface>(Owner);
		StateInterface = Cast<IDT_StateInterface>(Owner);

		CollisionManager->SetCharacter(Owner);
		CollisionManager->SetActorsToIgnore(Owner);
	}
}

void UDT_CombatComponent::SetHUDCrosshairs(float DeltaTime)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	APlayerController* Controller = Cast<APlayerController>(Character->GetController());
	
	if (Controller)
	{
		ADT_HUD* Hud = Cast<ADT_HUD>(Controller->GetHUD());
		if (Hud)
		{
			FCrosshairsTextures Textures;
			Textures = Weapon->GetCrosshairs();
			FVector2D WalkSpeedRange(0, Character->GetCharacterMovement()->MaxWalkSpeed);
			FVector2D VelocityMultiplierRange(0, 1);

			const float Value = (Character->GetCharacterMovement()->IsFalling()) ? 2.25f : 0.f;

			CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, UKismetMathLibrary::VSizeXY(Character->GetVelocity()));
			CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, Value, DeltaTime, 2.25f);

			CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, CrosshairZoom, DeltaTime, 30.f);
			CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.f, DeltaTime, 40.f);

			Textures.Spread = 0.5f + CrosshairVelocityFactor + CrosshairInAirFactor - CrosshairAimFactor + CrosshairShootingFactor;

			Hud->SetHUDPackage(Textures);

		}
	}
}

UMeshComponent* UDT_CombatComponent::GetWeaponMesh() const
{
	return (GetEquipWeapon()) ? Weapon->GetMeshComp() : nullptr;
}

FTransform UDT_CombatComponent::GetMeshSocketTransform(const FName& SocketName)
{
	USkeletalMeshComponent* MeshComp = Cast<USkeletalMeshComponent>(Weapon->GetMeshComp());
	return MeshComp->GetSocketTransform(SocketName, ERelativeTransformSpace::RTS_World);
}

void UDT_CombatComponent::ServerRPCAttack_Implementation(const FName& Section)
{
	MulticastRPCAttack(Section);
}

void UDT_CombatComponent::MulticastRPCAttack_Implementation(const FName& Section)
{
	UAnimMontage* PlayMontage = BaseAttackMontage;
	if (GetEquipWeapon() && IsValid(WeaponData))
		PlayMontage = WeaponData->AttackMontage;

	CombatInterface->PlayMontage(PlayMontage, Section);
}

void UDT_CombatComponent::ServerRPCDodge_Implementation(const FName& Section)
{
	MulticastRPCDodge(Section);
}

void UDT_CombatComponent::MulticastRPCDodge_Implementation(const FName& Section)
{
	CombatInterface->PlayMontage(DodgeMontage, Section);
}

void UDT_CombatComponent::ServerRPCHit_Implementation(const FName& Section)
{
	MulticastRPCHit(Section);
}

void UDT_CombatComponent::MulticastRPCHit_Implementation(const FName& Section)
{
	CombatInterface->PlayMontage(HitMontage, Section);
}

void UDT_CombatComponent::CreateWeapon(UDataAsset* DataAsset)
{
	WeaponData = Cast<UDA_Weapon>(DataAsset);
	if (IsValid(WeaponData) && IsValid(WeaponData->WeaponClass))
	{
		// 나중에 SpawnActorDeferred로 변경
		Weapon = GetWorld()->SpawnActor<ADT_BaseWeapon>(WeaponData->WeaponClass);
		Weapon->Equip(Cast<APawn>(GetOwner()), WeaponData->HolsterSocketName, CollisionManager);
	}
}

void UDT_CombatComponent::Equip(const bool bIsEquip, const FName& SectionName)
{
	EWeaponType WeaponType = EWeaponType::EWT_Default;
	if (bIsEquip)
		WeaponType = WeaponData->WeaponType;
	ServerRPCEquip(bIsEquip, SectionName, WeaponType);
}

void UDT_CombatComponent::ServerRPCEquip_Implementation(const bool bIsEquip, const FName& SectionName, const EWeaponType& WeaponType)
{
	MulticastRPCEquip(bIsEquip, SectionName, WeaponType);
}

void UDT_CombatComponent::MulticastRPCEquip_Implementation(const bool bIsEquip, const FName& SectionName, const EWeaponType& WeaponType)
{
	SetEquipWeapon(bIsEquip);
	StateInterface->SetEquipWeaponType(WeaponType);
	CombatInterface->PlayMontage(WeaponData->EquipMontage, SectionName);
}

void UDT_CombatComponent::ServerRPCAttachSocket_Implementation(const FName& SocketName)
{
	MulticastRPCAttachSocket(SocketName);
}

void UDT_CombatComponent::MulticastRPCAttachSocket_Implementation(const FName& SocketName)
{
	Weapon->AttachMeshToSocket(MeshInterface->GetMeshComp(), SocketName);
}

void UDT_CombatComponent::CollisionStart(const FDamagePacket& InDamagePacket)
{
	if (Cast<APawn>(GetOwner())->IsLocallyControlled())
	{
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		ServerRPCCollisionStart(InDamagePacket, HitResult.ImpactPoint);
		CrosshairShootingFactor = .75f;
		if (GetEquipWeapon() && StateInterface->GetEquipWeaponType() == EWeaponType::EWT_Gun)
			StartFireTimer(InDamagePacket);
	}
}

void UDT_CombatComponent::ServerRPCCollisionStart_Implementation(const FDamagePacket& DamagePacket, const FVector_NetQuantize& TraceHitTarget)
{
	if (GetEquipWeapon())
	{
		Weapon->Attack(DamagePacket, TraceHitTarget);
		MulticastRPCShowCosmetic(true);
	}
	else
	{
		CollisionManager->SetDamagePacket(DamagePacket);
		CollisionManager->SetDamage(BaseDamage);
		CollisionManager->DoCollision(GetOwner());
	}
}

void UDT_CombatComponent::CollisionEnd()
{
	if (Cast<APawn>(GetOwner())->IsLocallyControlled())
	{
		ServerRPCCollisionEnd();
	}
}

void UDT_CombatComponent::MulticastRPCShowCosmetic_Implementation(const bool bIsShow)
{
	Weapon->SetFXVisibility(bIsShow);
}

void UDT_CombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
		GEngine->GameViewport->GetViewportSize(ViewportSize);

	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection);

	if (bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;
		
		float DistanceToCharacter = (GetOwner()->GetActorLocation() - Start).Size();
		Start += CrosshairWorldDirection * (DistanceToCharacter + 100.f);
		FVector End = Start + CrosshairWorldDirection * 80000;

		GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECollisionChannel::ECC_Visibility);  
		if (!TraceHitResult.bBlockingHit)
			TraceHitResult.ImpactPoint = End;
	}
}

void UDT_CombatComponent::StartFireTimer(const FDamagePacket DamagePacket)
{
	IDT_GunInterface* Interface = Cast<IDT_GunInterface>(Weapon);
	FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &UDT_CombatComponent::FireTimerFinished, DamagePacket);

	GetOwner()->GetWorldTimerManager().SetTimer(TimerHandle, RespawnDelegate, Interface->GetAutoFireDelay(), false);
}

void UDT_CombatComponent::FireTimerFinished(const FDamagePacket DamagePacket)
{
	if (StateInterface->GetLMBDown())
		CollisionStart(DamagePacket);
}


void UDT_CombatComponent::ServerRPCCollisionEnd_Implementation()
{
	if (GetEquipWeapon())
	{
		Weapon->AttackEnd();
		MulticastRPCShowCosmetic(false);
	}
	else
		CollisionManager->StopCollision();
}

void UDT_CombatComponent::ServerRPCGuard_Implementation(const FName& Section)
{
	MulticastRPCGuard(Section);
}

void UDT_CombatComponent::MulticastRPCGuard_Implementation(const FName& Section)
{
	CombatInterface->PlayMontage(WeaponData->GuardMontage, Section);
}
