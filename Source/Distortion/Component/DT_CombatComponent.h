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
class ADT_BaseWeapon;
class UDT_CollisionManager;

DECLARE_DELEGATE_OneParam(FVisibleStatusDelegate, ESlateVisibility);
DECLARE_DELEGATE_OneParam(FImageDelegate, UTexture2D*);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DISTORTION_API UDT_CombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDT_CombatComponent();
	// virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void Attack(const FName& Section = "Attack01");
	UFUNCTION(Server, Unreliable)
	void ServerRPCAttack(const FName& Section);
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCAttack(const FName& Section);

	UFUNCTION()
	void Dodge(const FName& Section = "Fwd"){ ServerRPCDodge(Section); }
	UFUNCTION(Server, Unreliable)
	void ServerRPCDodge(const FName& Section);
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCDodge(const FName& Section);

	void CollisionStart(const FDamagePacket& DamagePacket);
	UFUNCTION(Server, Unreliable)
	void ServerRPCCollisionStart(const FDamagePacket& DamagePacket, const FVector_NetQuantize& TraceHitTarget);

	void CollisionEnd();
	UFUNCTION(Server, Unreliable)
	void ServerRPCCollisionEnd();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCShowCosmetic(const bool bIsShow);

	UFUNCTION()
	void Hit(const FName& SectionName, const EAttackType& AttackType) { ServerRPCHit(SectionName, AttackType); }
	UFUNCTION(Server, Unreliable)
	void ServerRPCHit(const FName& Section, const EAttackType& AttackType);
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCHit(const FName& Section, const EAttackType& AttackType);

	void CreateWeapon(UDataAsset* DataAsset);

	void EquipCheck() { (bEquipWeapon) ? Equip(false, "Unequip") : Equip(true, "Equip"); }

	UFUNCTION()
	void Equip(const bool bIsEquip, const FName& SectionName);
	UFUNCTION(Server, Unreliable)
	void ServerRPCEquip(const bool bIsEquip, const FName& SectionName, const EWeaponType& WeaponType);
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCEquip(const bool bIsEquip, const FName& SectionName, const EWeaponType& WeaponType);

	UFUNCTION()
	void Reload();
	UFUNCTION(Server, Unreliable)
	void ServerRPCReload();
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCReload();


	UFUNCTION()
	void AttachSocket(const FName& SocketName) {
		if (Cast<APawn>(GetOwner())->IsLocallyControlled())
			ServerRPCAttachSocket(SocketName); 
	}
	UFUNCTION(Server, Unreliable)
	void ServerRPCAttachSocket(const FName& SocketName);
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCAttachSocket(const FName& SocketName);

	void DestroyWeapon();

	FORCEINLINE float GetWeaponCost() const { return (GetEquipWeapon()) ? WeaponData->AttackCost : BaseAttackCost; }
	FORCEINLINE ADT_BaseWeapon* GetWeapon() const { return Weapon; }

public:
	FORCEINLINE void SetEquipWeapon(const bool InEquipWeapon) { bEquipWeapon = InEquipWeapon; }
	FORCEINLINE bool GetEquipWeapon() const { return bEquipWeapon; }
	FORCEINLINE bool GetHasEquipWeapon() const { return Weapon != nullptr; }
	FORCEINLINE UMeshComponent* GetWeaponMesh() const;
	FTransform GetMeshSocketTransform(const FName& SocketName);

protected:
	virtual void BeginPlay() override;

private:
	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> BaseAttackMontage;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> DodgeMontage;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> HitMontage;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> SmallHitMontage;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ADT_BaseWeapon> Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDA_Weapon> WeaponData;

	UPROPERTY()
	TScriptInterface<IDT_CombatInterface> CombatInterface;
	UPROPERTY()
	TScriptInterface<IDT_MeshInterface> MeshInterface;
	UPROPERTY()
	TScriptInterface<IDT_StateInterface> StateInterface;

	const float BaseDamage = 10.f;
	const float BaseAttackCost = 5.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bEquipWeapon = false;

	UPROPERTY()
	TObjectPtr<UDT_CollisionManager> CollisionManager;	
	UPROPERTY()
	TWeakObjectPtr<ACharacter> Character;
	UPROPERTY()
	TWeakObjectPtr<APlayerController> Controller;

public:
	FTimerHandle TimerHandle;
	void StartFireTimer();

public:
	UFUNCTION()
	void Guard(const FName& SectionName = "Default") { ServerRPCGuard(SectionName); }
	UFUNCTION(Server, Unreliable)
	void ServerRPCGuard(const FName& Section);
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCGuard(const FName& Section);

	FVisibleStatusDelegate VisibleStatus;
	FImageDelegate Image;

};
