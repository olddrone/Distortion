// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/DT_Attributes.h"
#include "DT_AttributeComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FDeadDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FHealthChangeDelegate, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FStaminaChangeDelegate, float);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DISTORTION_API UDT_AttributeComponent : public UActorComponent
{
	GENERATED_BODY()
	friend class ALT_PlayerController;

public:
	UDT_AttributeComponent();
	void InitValue();
	float ApplyDamage(const float Damage);

	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetStamina() const { return Attributes.Stamina; }

	void SetHealth(const float InHealth);

	FORCEINLINE float GetMaxHealth() const { return Attributes.MaxHealth; }
	FORCEINLINE float GetMaxStamina() const { return Attributes.MaxStamina; }

protected:
	virtual void InitializeComponent() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FORCEINLINE void SetStamina(const float InStamina) { Attributes.Stamina = FMath::Clamp<float>(InStamina, 0.0f, Attributes.MaxStamina); }

	UFUNCTION()
	void OnRep_Health();

public:
	FDeadDelegate Dead;
	FHealthChangeDelegate HealthChange;
	FStaminaChangeDelegate StaminaChange;

private:
	UPROPERTY(ReplicatedUsing = OnRep_Health, Transient, VisibleInstanceOnly)
	float Health;

	UPROPERTY(Transient, VisibleInstanceOnly, meta = (AllowPrivateAccess = "true"))
	FDT_Attributes Attributes;

};
