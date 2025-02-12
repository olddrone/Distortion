// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "DT_VM_EquipWeapon.generated.h"

class UDT_CombatComponent;
/**
 * 
 */
UCLASS()
class DISTORTION_API UDT_VM_EquipWeapon : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	void Init(UDT_CombatComponent* InCombat);
	void BindAmmo();

	UFUNCTION(BlueprintPure, FieldNotify)
	ESlateVisibility GetVisibilityStatus() const { return VisibilityStatus; }

	void SetVisibilityStatus(const ESlateVisibility InVisibilityStatus) {
		if (UE_MVVM_SET_PROPERTY_VALUE(VisibilityStatus, InVisibilityStatus))
			UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(VisibilityStatus);
	}

	UFUNCTION(BlueprintPure, FieldNotify)
	UTexture2D* GetWeaponImage() const { return WeaponImage; }

	void SetWeaponImage(UTexture2D* InWeaponImage) {
		if (UE_MVVM_SET_PROPERTY_VALUE(WeaponImage, InWeaponImage))
			UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(WeaponImage);
	}

	UFUNCTION(BlueprintPure, FieldNotify)
	uint8 GetAmmo() const { return Ammo; }

	void SetAmmo(const uint8 InAmmo) {
		if (UE_MVVM_SET_PROPERTY_VALUE(Ammo, InAmmo))
			UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(Ammo);
	}

	UFUNCTION(BlueprintPure, FieldNotify)
	ESlateVisibility GetAmmoVisibility() const { return AmmoVisibility; }

	void SetAmmoVisibility(const ESlateVisibility InAmmoVisibility) {
		if (UE_MVVM_SET_PROPERTY_VALUE(AmmoVisibility, InAmmoVisibility))
			UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(AmmoVisibility);
	}

protected:
	virtual void BindCallbacks();

protected:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	ESlateVisibility VisibilityStatus;

	UPROPERTY()
	TObjectPtr<UDT_CombatComponent> Combat;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	TObjectPtr<UTexture2D> WeaponImage;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	uint8 Ammo;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	ESlateVisibility AmmoVisibility;
	
};
