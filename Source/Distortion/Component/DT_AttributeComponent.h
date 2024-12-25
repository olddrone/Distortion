// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/DT_Attributes.h"
#include "DT_AttributeComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FDeadDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FHealthChangeDelegate, float);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DISTORTION_API UDT_AttributeComponent : public UActorComponent
{
	GENERATED_BODY()
	friend class ALT_PlayerController;

public:
	UDT_AttributeComponent();
	float ApplyDamage(const float Damage);
	FORCEINLINE float GetHealth() const { return Health; }

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetHealth(const float InHealth);

	UFUNCTION()
	void OnRep_Health();

public:
	FDeadDelegate Dead;
	FHealthChangeDelegate HealthChange;

private:
	UPROPERTY(ReplicatedUsing = OnRep_Health, Transient, VisibleInstanceOnly)
	float Health;

	UPROPERTY(Transient, VisibleInstanceOnly, meta = (AllowPrivateAccess = "true"))
	FDT_Attributes Attributes;
};
