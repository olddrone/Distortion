// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/DT_BaseWeapon.h"
#include "Interface/DT_GunInterface.h"
#include "DT_Gun.generated.h"

DECLARE_DELEGATE_OneParam(FAmmoChangeDelegate, uint8);
/**
 * 
 */
UCLASS()
class DISTORTION_API ADT_Gun : public ADT_BaseWeapon, public IDT_GunInterface
{
	GENERATED_BODY()
	friend class UDT_VM_EquipWeapon;

public:
	ADT_Gun();

	virtual void Equip(APawn* OwnerPawn, const FName& InSocketName, class UDT_CollisionManager* InCollisionManager) override;

	virtual void Attack(const FDamagePacket& DamagePacket, const FVector_NetQuantize& TraceHitTarget) override { }
	
	virtual void SetFXVisibility(const bool bVisible) override;

	virtual FCrosshairsTextures GetCrosshairs() const override { return Crosshairs; }

	virtual float GetAutoFireDelay() const override { return FireDelay; }
	virtual UAnimMontage* GetReloadMontage() const override { return ReloadMontage; }
	virtual uint8 GetAmmo() const override { return Ammo; }
	virtual uint8 GetMaxAmmo() const override { return MaxAmmo; }
	virtual void SetAmmo(const uint8 InAmmo) override;
	virtual void Load() override { Ammo = MaxAmmo; AmmoChange.ExecuteIfBound(Ammo); }
	virtual void DecreaseAmmo() override { SetAmmo(Ammo-1); }
	virtual void AmmoChangeEvent() override { AmmoChange.ExecuteIfBound(Ammo); }

	virtual void SetUI(const bool bIsEquip, class IDT_HUDInterface* Interface) override;
	virtual void SetScatterRadius(const float& InRad) override { ScatterRadius = InRad; }

protected:
	FORCEINLINE bool CanFire() { return (Ammo > 0) ? true : false; }
	void InitCrosshairsTexture();

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> FireAnimation;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> ReloadMontage;

	UPROPERTY(EditAnywhere)
	FCrosshairsTextures Crosshairs;

	UPROPERTY(EditAnywhere)
	float FireDelay = .15f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 MaxAmmo = 30;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 Ammo;

	float ScatterRadius;

	FAmmoChangeDelegate AmmoChange;
};
