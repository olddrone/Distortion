// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DT_CameraShakeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDT_CameraShakeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DISTORTION_API IDT_CameraShakeInterface
{
	GENERATED_BODY()

public:
	virtual void DoHitCameraShake() = 0;
};
