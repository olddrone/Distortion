// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DT_ObjectPooledInterface.generated.h"

UINTERFACE(MinimalAPI)
class UDT_ObjectPooledInterface : public UInterface
{
	GENERATED_BODY()
};


class DISTORTION_API IDT_ObjectPooledInterface
{
	GENERATED_BODY()

public:
	virtual void OnSpawnFromPool(const FVector_NetQuantize& Location, const FRotator& Rotation) = 0;
	virtual void OnReturnToPool() = 0;
};
