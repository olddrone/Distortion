// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DT_CombatInterface.generated.h"

UINTERFACE(MinimalAPI)
class UDT_CombatInterface : public UInterface
{
	GENERATED_BODY()
};

class DISTORTION_API IDT_CombatInterface
{
	GENERATED_BODY()

public:
	virtual void PlayMontage(class UAnimMontage* Montage, const FName& SectionName = "Default") = 0;

	virtual void DoAttack(const FName& Section) = 0;
	virtual void ActivateCollision(const struct FDamagePacket& DamagePacket) = 0;
	virtual void DeactivateCollision() = 0;

	virtual void GetHit(const FVector_NetQuantize& InstigatorLocation, const int8& DamageAmount,
		const FDamagePacket& DamagePacket) = 0;

	virtual class UDT_CombatComponent* GetCombatComponent() const = 0;

	virtual float GetSpread() const = 0;

};
