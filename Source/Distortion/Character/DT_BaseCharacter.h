// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/DT_StateInterface.h"
#include "Interface/DT_CombatInterface.h"
#include "Interface/DT_InteractionInterface.h"
#include "Interface/DT_MeshInterface.h"
#include "Interface/DT_AimOffsetInterface.h"
#include "DT_BaseCharacter.generated.h"

class UDT_AttributeComponent;
class UDT_CombatComponent;

UCLASS()
class DISTORTION_API ADT_BaseCharacter : public ACharacter,
	public IDT_StateInterface, public IDT_CombatInterface, public IDT_InteractionInterface, 
	public IDT_MeshInterface, public IDT_AimOffsetInterface
{
	GENERATED_BODY()

public:
	ADT_BaseCharacter();

	virtual EActionState GetActionState() const override { return ActionState; }
	virtual void SetActionState(const EActionState& State) override; // { ActionState = State; }

	virtual void RestoreState() override { if (GetEquipWeaponType() != EWeaponType::EWT_Gun) RMB(bRMBDown); }
	virtual void SetEquipWeaponType(const EWeaponType& WeaponType) override { EquipWeaponType = WeaponType; }
	virtual EWeaponType GetEquipWeaponType() const override { return EquipWeaponType; }

	virtual bool GetLMBDown() const override { return bLMBDown; }
	virtual bool GetRMBDown() const override { return bRMBDown; }

	virtual FTransform GetWeaponSocketTransform(const FName& SocketName) const override;
	virtual UDT_CombatComponent* GetCombatComponent() const override { return CombatComp; }

	virtual float GetSpread() const override { return 0.f; }



protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PlayMontage(UAnimMontage* Montage, const FName& SectionName = "Default") override;
	virtual UMeshComponent* GetMeshComp() const override { return GetMesh(); }
	virtual FVector GetSocketLocation(const FName& SocketName) const override { return GetMesh()->GetSocketLocation(SocketName); }

public:
	UFUNCTION()
	virtual void LMB(bool bIsAttack);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EWeaponType EquipWeaponType = EWeaponType::EWT_Default;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bLMBDown = false;

public:
	UFUNCTION()
	virtual void RMB(bool bHoldRotationYaw);

protected:
	UPROPERTY(Replicated)
	bool bRMBDown = false;

	UFUNCTION(Server, Unreliable)
	void ServerRPCRMBDown(bool bRMB);

	UFUNCTION()
	void SetRotationYaw(bool bHoldRotationYaw) { if (IsLocallyControlled()) ServerRPCSetRotationYaw(bHoldRotationYaw); }

	UFUNCTION(Server, Unreliable)
	void ServerRPCSetRotationYaw(bool bHoldRotationYaw);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCSetRotationYaw(bool bHoldRotationYaw);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	TObjectPtr<UDT_AttributeComponent> AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	TObjectPtr<UDT_CombatComponent> CombatComp;


public:
	UFUNCTION()
	void Dodge();

	UFUNCTION()
	virtual void Hit(const FName& SectionName, const EAttackType& AttackType);

	UFUNCTION()
	void ImmediateRotate();

	UFUNCTION(Server, Unreliable)
	void ServerRPCRotate(const FQuat4d& Quat);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCRotate(const FQuat4d& Quat);

	UFUNCTION()
	void Equip();

	UFUNCTION()
	void Reload();

public:
	virtual void DoAttack(const FName& SectionName = "Attack01") override;
	virtual void ActivateCollision(const FDamagePacket& DamagePacket) override;
	virtual void DeactivateCollision() override;

public:
	virtual void GetHit(const FVector_NetQuantize& InstigatorLocation, const int8& DamageAmount,
		const FDamagePacket& DamagePacket) override;

	UFUNCTION(Client, Unreliable)
	void ClientRPCGetHit(const FVector_NetQuantize& InstigatorLocation, const int8& DamageAmount,
		const FDamagePacket& DamagePacket);

	virtual void Interaction(UDataAsset* DataAsset) override;
	virtual void ToAttachSocket(const FName& SocketName) override;

public:
	void AnimTickOption(const EVisibilityBasedAnimTickOption& AnimTickOption);

	UFUNCTION(Server, Unreliable)
	void ServerRPCAnimTickOption(const EVisibilityBasedAnimTickOption& AnimTickOption);

public:
	void Guard(const FName& SectionName);
	virtual void Dead();

	virtual void SetTeamColor(const ETeam& Team) override;

	UPROPERTY(EditDefaultsOnly, Category = "TeamMaterials")
	TObjectPtr<UMaterialInterface> RedTeamMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "TeamMaterials")
	TObjectPtr<UMaterialInterface> BlueTeamMaterial;

	bool HasEnoughStamina(const float& Cost);
};