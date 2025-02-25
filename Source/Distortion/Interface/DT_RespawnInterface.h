// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DT_RespawnInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDT_RespawnInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DISTORTION_API IDT_RespawnInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void RequestPlayerRespawn(ACharacter* Character, AController* Controller) = 0;
	virtual void OnPlayerKill(AController* Controller) = 0;
};
