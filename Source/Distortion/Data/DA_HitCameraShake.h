// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_HitCameraShake.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FOscillation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Duration = 0.25f;
	
	UPROPERTY(EditAnywhere)
	float BlendInTime = 0.05f;
	
	UPROPERTY(EditAnywhere)
	float BlendOutTime = 0.05f;
};

USTRUCT(Atomic, BlueprintType)
struct FRotOscillationRange
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Min;
	UPROPERTY(EditAnywhere)
	float Max;
};

/**
 * 
 */
UCLASS()
class DISTORTION_API UDA_HitCameraShake : public UDataAsset
{
	GENERATED_BODY()
	friend class UDT_LegacyCameraShake;

protected:
	UPROPERTY(EditAnywhere)
	FOscillation Oscillation;

	UPROPERTY(EditAnywhere)
	FRotOscillationRange Amplitude;

	UPROPERTY(EditAnywhere)
	FRotOscillationRange Frequency;
};
