// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/DA_Weapon.h"
#include "Data/DT_DamageData.h"
#include "Interface/DT_CombatInterface.h"
#include "Interface/DT_MeshInterface.h"
#include "Interface/DT_StateInterface.h"
#include "DT_CombatComponent.generated.h"

class UAnimMontage;
class UDT_CollisionManager;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DISTORTION_API UDT_CombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDT_CombatComponent();

	UFUNCTION()
	void Attack(const FName& Section = "Attack01");
	UFUNCTION(Server, Unreliable)
	void ServerRPCAttack(const FName& Section);
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCAttack(const FName& Section);

	UFUNCTION()
	void Dodge(const FName& Section = "Fwd");
	UFUNCTION(Server, Unreliable)
	void ServerRPCDodge(const FName& Section);
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCDodge(const FName& Section);

	void CollisionStart(const FDamagePacket& DamagePacket);
	void CollisionEnd();

	UFUNCTION()
	void Hit(const FName& SectionName = "Fwd") { ServerRPCHit(SectionName); }
	UFUNCTION(Server, Unreliable)
	void ServerRPCHit(const FName& SectionName);
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCHit(const FName& SectionName);

	void CreateWeapon(UDataAsset* DataAsset);
	void EquipCheck() { (bEquipWeapon) ? Equip(false, "Unequip") : Equip(true, "Equip"); }

	UFUNCTION()
	void Equip(const bool bIsEquip, const FName& SectionName);
	UFUNCTION(Server, Unreliable)
	void ServerRPCEquip(const bool bIsEquip, const FName& SectionName, const EWeaponType& WeaponType);
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCEquip(const bool bIsEquip, const FName& SectionName, const EWeaponType& WeaponType);

	UFUNCTION()
	void AttachSocket(const FName& SocketName) {
		if (Cast<APawn>(GetOwner())->IsLocallyControlled())
			ServerRPCAttachSocket(SocketName); 
	}
	UFUNCTION(Server, Unreliable)
	void ServerRPCAttachSocket(const FName& SocketName);
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCAttachSocket(const FName& SocketName);

public:
	FORCEINLINE void SetEquipWeapon(const bool InEquipWeapon) { bEquipWeapon = InEquipWeapon; }
	FORCEINLINE bool GetEquipWeapon() const { return bEquipWeapon; }
	FORCEINLINE bool GetHasEquipWeapon() const { return Weapon != nullptr; }
	FORCEINLINE UMeshComponent* GetWeaponMesh() const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> BaseAttackMontage;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> DodgeMontage;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> HitMontage;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ADT_BaseWeapon> Weapon;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDA_Weapon> WeaponData;

	IDT_CombatInterface* CombatInterface;
	IDT_MeshInterface* MeshInterface;
	IDT_StateInterface* StateInterface;

	const int8 BaseDamage = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bEquipWeapon = false;

	UPROPERTY()
	TObjectPtr<UDT_CollisionManager> CollisionManager;


public:
	UFUNCTION()
	void SetFXVisibility(const bool bVisible);
	UFUNCTION(Server,Unreliable)
	void ServerPRCSetFXVisibility(const bool bVisible);
	UFUNCTION(NetMulticast,Unreliable)
	void MulticastRPCSetFXVisibility(const bool bVisible);
};
