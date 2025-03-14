// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DT_AimOffsetInterface.generated.h"

UINTERFACE(MinimalAPI)
class UDT_AimOffsetInterface : public UInterface
{
	GENERATED_BODY()
};

class DISTORTION_API IDT_AimOffsetInterface
{
	GENERATED_BODY()
public:
	virtual FTransform GetWeaponSocketTransform(const FName& SocketName) const = 0;
};
