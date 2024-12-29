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
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnSpawnFromPool();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnReturnToPool();
};
