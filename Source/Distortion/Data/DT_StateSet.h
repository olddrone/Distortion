// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unocuupied	UMETA(DisplayName = "Unocuupied"),
	EAS_Hit			UMETA(DisplayName = "Hit"),
	EAS_Attack		UMETA(DisplayName = "Attacking"),
	EAS_Equip		UMETA(DisplayName = "Equipping"),
	EAS_Dodge		UMETA(DisplayName = "Dodge"),
	EAS_Dead		UMETA(DisplayName = "Dead"),
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_Default		UMETA(DisplayName = "Default"),
	EWT_Sword		UMETA(DisplayName = "Sword"),
	EWT_Gun 		UMETA(DisplayName = "Gun"),
};

namespace WalkSpeed
{
	const float Walk = 350.f;
	const float Run = 450.f;
}; 

UENUM(BlueprintType)
enum class ETurnInPlace : uint8
{
	ETIP_NotTurn	UMETA(DisplayName = "Not Turning"),
	ETIP_Left		UMETA(DisplayName = "Turning Left"),
	ETIP_Right		UMETA(DisplayName = "Turning Right")
};