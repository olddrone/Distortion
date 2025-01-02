// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_CombatComponent.h"
#include "Component/DT_CollisionManager.h"
#include "Weapon/DT_BaseWeapon.h"
#include "Library/DT_CustomLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

UDT_CombatComponent::UDT_CombatComponent()
{
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

void UDT_CombatComponent::BeginPlay()
{
	Super::BeginPlay();

	CombatInterface = Cast<IDT_CombatInterface>(GetOwner());
	MeshInterface = Cast<IDT_MeshInterface>(GetOwner());
	StateInterface = Cast<IDT_StateInterface>(GetOwner());

	CollisionManager->SetCharacter(GetOwner());
	CollisionManager->SetActorsToIgnore(GetOwner());
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

void UDT_CombatComponent::CollisionStart(const FDamagePacket& DamagePacket)
{
	if (Cast<APawn>(GetOwner())->IsLocallyControlled())
	{
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		ServerRPCCollisionStart(DamagePacket, HitResult.ImpactPoint);
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
		CollisionManager->SetSocketName(DamagePacket.StartSocketName, DamagePacket.EndSocketName);
		CollisionManager->DoCollision(GetOwner());
	}
}

void UDT_CombatComponent::CollisionEnd()
{
	if (Cast<APawn>(GetOwner())->IsLocallyControlled())
		ServerRPCCollisionEnd();
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
		FVector End = Start + CrosshairWorldDirection * 80000;
		GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECollisionChannel::ECC_Visibility);  
		if (!TraceHitResult.bBlockingHit)
			TraceHitResult.ImpactPoint = End;
	}
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
