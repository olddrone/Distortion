// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_VM_EquipWeapon.h"
#include "Component/DT_CombatComponent.h"	
#include "Blueprint/UserWidget.h"

#include "Weapon/DT_BaseWeapon.h"
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
}

void UDT_VM_EquipWeapon::BindAmmo()
{
	// BaseWeapon 및 인터페이스로 추상화 필요
	ADT_Gun* Gun = Cast<ADT_Gun>(Combat->GetWeapon());
	if (IsValid(Gun))
		Gun->AmmoChange.BindUObject(this, &UDT_VM_EquipWeapon::SetAmmo);
}

void UDT_VM_EquipWeapon::BindAmmoVisibility()
{
	ADT_BaseWeapon* Weapon = Cast<ADT_BaseWeapon>(Combat->GetWeapon());
	if (IsValid(Weapon))
		Weapon->AmmoVisible.BindUObject(this, &UDT_VM_EquipWeapon::SetAmmoVisibility);
}
