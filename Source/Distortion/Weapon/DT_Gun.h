// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/DT_BaseWeapon.h"
#include "Interface/DT_GunInterface.h"
#include "DT_Gun.generated.h"

/**
 * 
 */
UCLASS()
class DISTORTION_API ADT_Gun : public ADT_BaseWeapon, public IDT_GunInterface
{
	GENERATED_BODY()

public:
	ADT_Gun();

	virtual void Tick(float DeltaTime) override;

	virtual void Attack(const FDamagePacket& DamagePacket, const FVector_NetQuantize& TraceHitTarget) override { }
	virtual void SetFXVisibility(const bool bVisible) override;

	virtual FCrosshairsTextures GetCrosshairs() const override { return Crosshairs; }

	virtual float GetAutoFireDelay() const override { return FireDelay; }

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimationAsset> FireAnimation;

	UPROPERTY(EditAnywhere)
	FCrosshairsTextures Crosshairs;

	UPROPERTY(EditAnywhere)
	float FireDelay = .15f;
};
