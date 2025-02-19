// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DT_DamageData.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	EAT_Default		UMETA(DisplayName = "Default"),
	EAT_Light		UMETA(DisplayName = "Light"),
	EAT_Heavy		UMETA(DisplayName = "Heavy"),
	EAT_Bullet		UMETA(DisplayName = "Bullet")
};

UENUM(BlueprintType)
enum class EAttackDirection : uint8
{
	EAD_Default		UMETA(DisplayName = "Default"),
	EAD_Forward		UMETA(DisplayName = "Forward"),
	EAD_Right 		UMETA(DisplayName = "Right"),
	EAD_Left 		UMETA(DisplayName = "Left"),
	EAD_Shot 		UMETA(DisplayName = "Shot")
};

USTRUCT(Atomic, BlueprintType)
struct FDamagePacket
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttackType AttackType;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttackDirection AttackDirection;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName StartSocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName EndSocketName;

	FDamagePacket& operator=(std::nullptr_t) {
		AttackType = EAttackType::EAT_Default;
		AttackDirection = EAttackDirection::EAD_Default;
		StartSocketName = EndSocketName = FName("");

		return *this;
	}
};