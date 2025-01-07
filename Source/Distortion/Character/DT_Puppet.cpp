// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DT_Puppet.h"
#include "Component/DT_CombatComponent.h"

void ADT_Puppet::BeginPlay()
{
	Super::BeginPlay();
	SpawnDefaultWeapon();

	Equip();

}

void ADT_Puppet::SpawnDefaultWeapon()
{

	if (!CombatComp->GetHasEquipWeapon())
		CombatComp->CreateWeapon(TmpWeapon);
}