// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Data/DT_StateSet.h"
#include "DT_StateInterface.generated.h"

UINTERFACE(MinimalAPI)
class UDT_StateInterface : public UInterface
{
	GENERATED_BODY()
};

class DISTORTION_API IDT_StateInterface
{
	GENERATED_BODY()

public:
	virtual EActionState GetActionState() const = 0;
	virtual void SetActionState(const EActionState& State) = 0;
	virtual void RestoreState() = 0;

	virtual void SetEquipWeaponType(const EWeaponType& WeaponType) = 0;
	virtual EWeaponType GetEquipWeaponType() const = 0;

	virtual bool GetLMBDown() const = 0;
	virtual bool GetRMBDown() const = 0;
};
