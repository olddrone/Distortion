// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "Interface/DT_CameraControlInterface.h"
#include "DT_CameraManager.generated.h"

/**
 * 
 */
UCLASS()
class DISTORTION_API ADT_CameraManager : public APlayerCameraManager, public IDT_CameraControlInterface
{
	GENERATED_BODY()

public:
	ADT_CameraManager();

	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
	virtual void DoHitCameraShake() override;
	virtual void SetZoom(const bool& bInZoom) override { bIsZooming = bInZoom; }

private:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<ACharacter> Character;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<class UCharacterMovementComponent> MoveComp;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float CrouchBlendDuration = .5f;
	float CrouchBlendTime;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float ZoomedFOV = 30.0f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float ZoomInterpSpeed = 20.0f;

	float InterpFOV;
	bool bIsZooming = false; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> HitShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> AttackShake;
};
