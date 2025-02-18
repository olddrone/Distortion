// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/DT_BaseWeapon.h"
#include "DT_Sword.generated.h"

class UDT_CollisionManager;

/**
 * 
 */
UCLASS()
class DISTORTION_API ADT_Sword : public ADT_BaseWeapon
{
	GENERATED_BODY()
public:
	ADT_Sword();

	virtual void Attack(const FDamagePacket& DamagePacket, const FVector_NetQuantize& TraceHitTarget) override;
	virtual void AttackEnd() override;

	virtual void Equip(APawn* OwnerPawn, const FName& InSocketName, UDT_CollisionManager* InCollisionManager) override;

	virtual void SetFXVisibility(const bool bVisible)  override;

	virtual void SetUI(const bool bIsEquip, class IDT_HUDInterface* Interface) override;
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDT_CollisionManager> CollisionManager;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> TrailEffect;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float Damage;
};
