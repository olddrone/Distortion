// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DT_CrosshairComponent.generated.h"

USTRUCT(BlueprintType)
struct FCHFactor
{
	GENERATED_BODY()

	float Velocity;
	float InAir;
	float Aim;
	float Shooting;
	float Zoom;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DISTORTION_API UDT_CrosshairComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDT_CrosshairComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE void SetAimFactor(const float InZoom) { CrosshairFactor.Zoom = InZoom; }
	FORCEINLINE void SetShootingFactor(const float InShoot) { CrosshairFactor.Shooting = InShoot; }
	FORCEINLINE float GetSpread() const { return Spread; }

protected:
	virtual void BeginPlay() override;

	void SetHUDCrosshairs(float DeltaTime);

protected:	

	UPROPERTY()
	TWeakObjectPtr<ACharacter> Character;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> Controller;

	UPROPERTY()
	TObjectPtr<class UDT_CombatComponent> CombatComponent;
	
	FCHFactor CrosshairFactor;
	
	const float SpreadMax = 15.f;
	float Spread;

	UPROPERTY()
	TScriptInterface<class IDT_HUDInterface> HUDInterface;

	UPROPERTY()
	TScriptInterface<class IDT_StateInterface> StateInterface;
};
