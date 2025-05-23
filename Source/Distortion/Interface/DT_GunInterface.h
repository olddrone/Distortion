// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DT_GunInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDT_GunInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DISTORTION_API IDT_GunInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetAutoFireDelay() const = 0;
	virtual UAnimMontage* GetReloadMontage() const = 0;
	virtual uint8 GetAmmo() const = 0;
	virtual uint8 GetMaxAmmo() const = 0;
	virtual void SetAmmo(const uint8 InAmmo) = 0;
	virtual void Load() = 0;
	virtual void DecreaseAmmo() = 0;
	virtual void AmmoChangeEvent() = 0;
	virtual void SetScatterRadius(const float& InRad) = 0;
};
