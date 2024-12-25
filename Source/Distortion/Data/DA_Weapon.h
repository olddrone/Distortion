// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Data/DT_StateSet.h"
#include "DA_Weapon.generated.h"

/**
 * 
 */
UCLASS()
class DISTORTION_API UDA_Weapon : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ADT_BaseWeapon> WeaponClass;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> EquipMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere)
	FName EquipSocketName;

	UPROPERTY(EditAnywhere)
	FName HolsterSocketName;
};
