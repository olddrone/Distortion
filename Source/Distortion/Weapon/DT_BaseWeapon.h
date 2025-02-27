// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/DT_DamageData.h"
#include "Data/DT_Crosshairs.h"
#include "Interface/DT_MeshInterface.h"
#include "DT_BaseWeapon.generated.h"

DECLARE_DELEGATE_OneParam(FAmmoVisibleDelegate, ESlateVisibility);

UCLASS(Abstract)
class DISTORTION_API ADT_BaseWeapon : public AActor, public IDT_MeshInterface
{
	GENERATED_BODY()
	
public:
	ADT_BaseWeapon();

	virtual void Attack(const FDamagePacket& DamagePacket, const FVector_NetQuantize& TraceHitTarget) PURE_VIRTUAL(ADT_BaseWeapon::Attack, );
	virtual void AttackEnd() {}
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

	virtual void Equip(APawn* OwnerPawn, const FName& InSocketName, class UDT_CollisionManager* InCollisionManager);

	virtual UMeshComponent* GetMeshComp() const override { return MeshComponent; }
	virtual FVector GetSocketLocation(const FName& SocketName) const override;
	virtual void SetFXVisibility(const bool bVisible)  PURE_VIRTUAL(ADT_BaseWeapon::SetFXVisibility, );

	virtual FCrosshairsTextures GetCrosshairs() const { return FCrosshairsTextures(nullptr); }

	virtual void SetUI(const bool bIsEquip, class IDT_HUDInterface* Interface);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, meta = (AllowedClasses = "StaticMeshComponent, SkeletalMeshComponent"))
	TObjectPtr<UMeshComponent> MeshComponent;

public:
	FAmmoVisibleDelegate AmmoVisible;
};
