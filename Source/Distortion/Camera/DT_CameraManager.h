// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "DT_CameraManager.generated.h"

/**
 * 
 */
UCLASS()
class DISTORTION_API ADT_CameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float CrouchBlendDuration = .5f;

	float CrouchBlendTime;


public:
	void SetZoomState(bool bInZoom) { bIsZooming = bInZoom; }

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float ZoomedFOV = 45.0f;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float ZoomInterpSpeed = 20.0f;

	float TestFOV;

	bool bIsZooming = false; 
};
