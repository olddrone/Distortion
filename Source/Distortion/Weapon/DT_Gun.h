// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/DT_BaseWeapon.h"
#include "DT_Gun.generated.h"

/**
 * 
 */
UCLASS()
class DISTORTION_API ADT_Gun : public ADT_BaseWeapon
{
	GENERATED_BODY()

public:
	ADT_Gun();

	virtual void Attack(const FDamagePacket& DamagePacket) override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimationAsset> FireAnimation;
};
