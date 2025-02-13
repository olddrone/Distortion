// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_VM_EquipWeapon.h"
#include "Component/DT_CombatComponent.h"	
#include "Blueprint/UserWidget.h"
#include "Weapon/DT_Gun.h"

void UDT_VM_EquipWeapon::Init(UDT_CombatComponent* InCombat)
{
	Combat = Cast<UDT_CombatComponent>(InCombat);
	BindCallbacks();
}

void UDT_VM_EquipWeapon::BindCallbacks()
{
	if (!ensure(Combat))
		return;

	SetVisibilityStatus(ESlateVisibility::Hidden);
	Combat->VisibleStatus.BindUObject(this, &UDT_VM_EquipWeapon::SetVisibilityStatus);
	Combat->Image.BindUObject(this, &UDT_VM_EquipWeapon::SetWeaponImage);
	Combat->AmmoVisible.BindUObject(this, &UDT_VM_EquipWeapon::SetAmmoVisibility);
}

void UDT_VM_EquipWeapon::BindAmmo()
{
	ADT_Gun* Gun = Cast<ADT_Gun>(Combat->GetWeapon());
	if (IsValid(Gun))
		Gun->AmmoChange.BindUObject(this, &UDT_VM_EquipWeapon::SetAmmo);
}
