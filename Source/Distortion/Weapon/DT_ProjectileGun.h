// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/DT_Gun.h"
#include "DT_ProjectileGun.generated.h"

/**
 * 
 */
UCLASS()
class DISTORTION_API ADT_ProjectileGun : public ADT_Gun
{
	GENERATED_BODY()
	
public:
	ADT_ProjectileGun();


public:
	virtual void Attack(const FDamagePacket& DamagePacket, const FVector_NetQuantize& TraceHitTarget) override final;
	
private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<AActor> ProjectileClass;
};
