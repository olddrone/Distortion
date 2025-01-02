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
	public IDT_StateInterface, public IDT_CombatInterface, public IDT_InteractionInterface, public IDT_MeshInterface,
	public IDT_AimOffsetInterface
{
	GENERATED_BODY()

public:
	ADT_BaseCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void OnRep_ReplicatedMovement() override;

	virtual EActionState GetActionState() const override { return ActionState; }
	virtual void SetActionState(const EActionState& State) override { ActionState = State; }
	virtual void RestoreState() override { if (GetEquipWeaponType() != EWeaponType::EWT_Gun) SetRotationYaw(bRMBDown); }
	virtual void SetEquipWeaponType(const EWeaponType& WeaponType) override { EquipWeaponType = WeaponType; }
	virtual EWeaponType GetEquipWeaponType() const override { return EquipWeaponType; }

	virtual bool GetLMBDown() const override { return bLMBDown; }
	virtual void SetLMBDown(const bool& bDown) override { bLMBDown = bDown; }
	virtual bool GetRMBDown() const override { return bRMBDown; }

	virtual float GetAimOffsetYaw() const override { return AimOffsetYaw; }
	virtual FTransform GetWeaponSocketTransform(const FName& SocketName) const override;
	virtual ETurnInPlace GetTurnInPlace() const override { return TurnInPlace; }
	FORCEINLINE UDT_CombatComponent* GetCombatComponent() const { return CombatComp; }
	virtual bool ShouldRotateRootBone() const override { return bRotateRootBone; }

protected:
	virtual void BeginPlay() override;
	void AimOffset(const float& DeltaTime);
	void CheckTurnInPlace(const float& DeltaTime);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PlayMontage(UAnimMontage* Montage, const FName& SectionName = "Default") override;
	virtual UMeshComponent* GetMeshComp() const override { return GetMesh(); }
	virtual FVector GetSocketLocation(const FName& SocketName) const override { return GetMesh()->GetSocketLocation(SocketName); }

public:
	UFUNCTION()
	void RMB(bool bHoldRotationYaw);

	UFUNCTION(Server, Unreliable)
	void ServerRPCRMBDown(bool bHoldRotationYaw);

	UFUNCTION()
	void SetRotationYaw(bool bHoldRotationYaw);

	UFUNCTION(Server, Unreliable)
	void ServerRPCSetRotationYaw(bool bHoldRotationYaw);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCSetRotationYaw(bool bHoldRotationYaw);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EActionState ActionState = EActionState::EAS_Unocuupied;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EWeaponType EquipWeaponType = EWeaponType::EWT_Default;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bLMBDown = false;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite)
	bool bRMBDown = false;

	float AimOffsetYaw = 0.f;
	float InterpAOYaw;
	FRotator StartingAimRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ETurnInPlace TurnInPlace = ETurnInPlace::ETIP_NotTurn;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TObjectPtr<UDT_AttributeComponent> AttributeComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TObjectPtr<UDT_CombatComponent> CombatComp;

public:
	UFUNCTION()
	void Attack();

	UFUNCTION()
	void Dodge();

	UFUNCTION()
	void Hit(const FName& SectionName);

	UFUNCTION()
	void ImmediateRotate();

	UFUNCTION(Server, Unreliable)
	void ServerRPCRotate(const FQuat4d& Quat);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCRotate(const FQuat4d& Quat);

	UFUNCTION()
	void Equip();

public:
	virtual void DoAttack(const FName& SectionName = "Attack01") override;
	virtual void ActivateCollision(const FDamagePacket& DamagePacket) override;
	virtual void DeactivateCollision() override;

public:
	virtual void GetHit(const FVector_NetQuantize& InstigatorLocation, const int8& DamageAmount) override;

	UFUNCTION(Client, Unreliable)
	void ClientRPCGetHit(const FVector_NetQuantize& InstigatorLocation, const int8& DamageAmount);

	virtual void Interaction(UDataAsset* DataAsset) override;
	virtual void ToAttachSocket(const FName& SocketName) override;

public:
	void SimProxiesTurn();

	void AnimTickOption(const EVisibilityBasedAnimTickOption& AnimTickOption);

	UFUNCTION(Server, Unreliable)
	void ServerRPCAnimTickOption(const EVisibilityBasedAnimTickOption& AnimTickOption);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCAnimTickOption(const EVisibilityBasedAnimTickOption& AnimTickOption);

private:
	bool bRotateRootBone;
	float TurnThreshold = 0.5f;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;
	float ProxyYaw;
	float TimeSinceLastMovementReplication;


};
