// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_CombatComponent.h"
#include "Component/DT_CollisionManager.h"
#include "Weapon/DT_BaseWeapon.h"
#include "Library/DT_CustomLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Data/DT_Crosshairs.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/HUD.h"
#include "Interface/DT_HUDInterface.h"
#include "TimerManager.h"
#include "Interface/DT_GunInterface.h"
#include "Blueprint/UserWidget.h"
#include "DrawDebugHelpers.h"

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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SmallHitRef(
		TEXT("/Game/Blueprints/Player/Animations/AM_SmallHit.AM_SmallHit"));
	if (SmallHitRef.Object)
		SmallHitMontage = SmallHitRef.Object;

	CollisionManager = CreateDefaultSubobject<UDT_CollisionManager>(TEXT("CollisionManager"));
}

void UDT_CombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SetHUDCrosshairs(DeltaTime);
}

void UDT_CombatComponent::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<ACharacter>(GetOwner()); 
	Controller = Cast<APlayerController>(Character->GetController());

	if (Character.IsValid())
	{
		CombatInterface = Cast<IDT_CombatInterface>(Character);
		MeshInterface = Cast<IDT_MeshInterface>(Character);
		StateInterface = Cast<IDT_StateInterface>(Character);

		CollisionManager->SetOwner(Cast<APawn>(Character));
		CollisionManager->SetActorsToIgnore(Character.Get());
	}
}

void UDT_CombatComponent::SetHUDCrosshairs(float DeltaTime)
{
	if (Controller.IsValid())
	{
		IDT_HUDInterface* HUDInterface = Cast<IDT_HUDInterface>(Controller.Get()->GetHUD());
		if (HUDInterface)
		{
			FCrosshairsTextures Textures;

			if (GetEquipWeapon())
			{
				Textures = Weapon->GetCrosshairs();
				FVector2D WalkSpeedRange(0, Character->GetCharacterMovement()->MaxWalkSpeed);
				FVector2D VelocityMultiplierRange(0, 1);

				const float Value = (Character->GetCharacterMovement()->IsFalling()) ? 2.25f : 0.f;

				CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, UKismetMathLibrary::VSizeXY(Character->GetVelocity()));
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, Value, DeltaTime, 2.25f);

				CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, CrosshairZoom, DeltaTime, 30.f);
				CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.f, DeltaTime, 40.f);
				
				Spread = 0.5f + CrosshairVelocityFactor + CrosshairInAirFactor - CrosshairAimFactor + CrosshairShootingFactor;
				Spread *= CrosshairSpreadMax;
				Textures.Spread = Spread ;
			}
			HUDInterface->SetHUDPackage(Textures);
		}
	}
}

void UDT_CombatComponent::DestroyWeapon()
{
	WeaponData = nullptr;
	if (IsValid(Weapon)) 
		Weapon->Destroy();

	VisibleStatus.ExecuteIfBound(ESlateVisibility::Hidden);
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

void UDT_CombatComponent::Attack(const FName& Section)
{
	IDT_GunInterface* Interface = Cast<IDT_GunInterface>(Weapon);
	if (Interface)
	{
		Interface->DecreaseAmmo();
		if (Interface->GetAmmo() <= 0)
		{
			Reload();
			return;
		}
	}

	ServerRPCAttack(Section);

	if (GetEquipWeapon() && StateInterface->GetEquipWeaponType() == EWeaponType::EWT_Gun)
		StartFireTimer();
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

void UDT_CombatComponent::ServerRPCHit_Implementation(const FName& Section, const EAttackType& AttackType)
{
	MulticastRPCHit(Section,AttackType);
}

void UDT_CombatComponent::MulticastRPCHit_Implementation(const FName& Section, const EAttackType& AttackType)
{
	UAnimMontage* PlayMontage = (AttackType == EAttackType::EAT_Bullet) ? SmallHitMontage : HitMontage;
	CombatInterface->PlayMontage(PlayMontage, Section);
}

void UDT_CombatComponent::CreateWeapon(UDataAsset* DataAsset)
{
	WeaponData = Cast<UDA_Weapon>(DataAsset);
	if (IsValid(WeaponData) && IsValid(WeaponData->WeaponClass))
	{
		FTransform SpawnTransform = FTransform::Identity;
		ADT_BaseWeapon* TempWeapon = GetWorld()->SpawnActorDeferred<ADT_BaseWeapon>(
			WeaponData->WeaponClass, SpawnTransform);
		if (TempWeapon)
		{
			TempWeapon->FinishSpawning(SpawnTransform);
			TempWeapon->Equip(Cast<APawn>(GetOwner()), WeaponData->HolsterSocketName, CollisionManager);
			Weapon = TempWeapon;
		}
	}
}

void UDT_CombatComponent::Equip(const bool bIsEquip, const FName& SectionName)
{
	EWeaponType WeaponType = EWeaponType::EWT_Default;
	if (bIsEquip)
		WeaponType = WeaponData->WeaponType;
	ServerRPCEquip(bIsEquip, SectionName, WeaponType);

	VisibleStatus.ExecuteIfBound((bIsEquip) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	Image.ExecuteIfBound(WeaponData->WeaponImage);

	IDT_HUDInterface* HUDInterface = Cast<IDT_HUDInterface>(Controller->GetHUD());
	Weapon->SetUI(bIsEquip, HUDInterface);
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

void UDT_CombatComponent::Reload()
{
	ServerRPCReload();
}

void UDT_CombatComponent::ServerRPCReload_Implementation()
{
	MulticastRPCReload();
}

void UDT_CombatComponent::MulticastRPCReload_Implementation()
{
	IDT_GunInterface* GunInterface = Cast<IDT_GunInterface>(Weapon);
	if (GunInterface)
	{
		CombatInterface->PlayMontage(GunInterface->GetReloadMontage());
		GunInterface->Load();
	}
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
		CrosshairShootingFactor = .75f;
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);

		ServerRPCCollisionStart(InDamagePacket, HitResult.ImpactPoint);
		
	}
}

void UDT_CombatComponent::ServerRPCCollisionStart_Implementation(const FDamagePacket& DamagePacket, const FVector_NetQuantize& TraceHitTarget)
{
	if (GetEquipWeapon())
	{
		Weapon->Attack(DamagePacket, TraceHitTarget); // 서버에서 호출함
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

		float DistanceToCharacter = (GetOwner()->GetActorLocation() - Start).Size();
		Start += CrosshairWorldDirection * (DistanceToCharacter + 100.f);
		FVector End = Start + CrosshairWorldDirection * 50000;

		GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECollisionChannel::ECC_Visibility);
		if (!TraceHitResult.bBlockingHit)
			TraceHitResult.ImpactPoint = End;

		// Scatter 구현
		float CameraFOV = Controller->PlayerCameraManager->GetFOVAngle();
		float DistanceToImpact = (End - Start).Size();
		float AdjustedRadius = (DistanceToImpact / DistanceToCharacter) * Spread * (90.0f / CameraFOV);
		
		FVector RandomOffset = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0, AdjustedRadius*2);
		FVector ScatterEnd = End + RandomOffset;
		TraceHitResult.ImpactPoint = ScatterEnd;

		DrawDebugLine(GetWorld(), Start, TraceHitResult.ImpactPoint, FColor::Green, true);
	}
}

void UDT_CombatComponent::StartFireTimer()
{
	IDT_GunInterface* GunInterface = Cast<IDT_GunInterface>(Weapon);
	if (GunInterface->GetAmmo() <= 0)
		return;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]() {
		if (StateInterface->GetLMBDown()) {
			StateInterface->SetActionState(EActionState::EAS_Unoccupied);
			CombatInterface->DoAttack(StateInterface->GetRMBDown() ? "Ironsight" : "Hip");
		} }), GunInterface->GetAutoFireDelay(), false);
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
