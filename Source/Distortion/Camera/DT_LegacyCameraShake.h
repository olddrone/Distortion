// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shakes/LegacyCameraShake.h"
#include "Data/DA_HitCameraShake.h"
#include "DT_LegacyCameraShake.generated.h"

/**
 * 
 */
UCLASS()
class DISTORTION_API UDT_LegacyCameraShake : public ULegacyCameraShake
{
	GENERATED_BODY()
	
public:
	UDT_LegacyCameraShake();

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDA_HitCameraShake> ShakeData;
};
