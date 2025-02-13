// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DT_CameraControlInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDT_CameraControlInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DISTORTION_API IDT_CameraControlInterface
{
	GENERATED_BODY()

public:
	virtual void DoHitCameraShake() = 0;
	virtual void SetZoom(const bool& bInZoom) = 0;
};
