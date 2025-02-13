// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_LegacyCameraShake.h"
#include "UObject/ConstructorHelpers.h"

UDT_LegacyCameraShake::UDT_LegacyCameraShake()
{
	static ConstructorHelpers::FObjectFinder<UDA_HitCameraShake> DataRef(
		TEXT("/Game/Data/DA_HitCameraShake.DA_HitCameraShake"));
	if (DataRef.Succeeded())
		ShakeData = DataRef.Object;

	OscillationDuration = ShakeData->Oscillation.Duration;
	OscillationBlendInTime = ShakeData->Oscillation.BlendInTime;
	OscillationBlendOutTime = ShakeData->Oscillation.BlendOutTime;

	RotOscillation.Pitch.Amplitude = FMath::RandRange(ShakeData->Amplitude.Min, ShakeData->Amplitude.Max);
	RotOscillation.Pitch.Frequency = FMath::RandRange(ShakeData->Frequency.Min, ShakeData->Frequency.Max);

	RotOscillation.Yaw.Amplitude = FMath::RandRange(ShakeData->Amplitude.Min, ShakeData->Amplitude.Max);
	RotOscillation.Yaw.Frequency = FMath::RandRange(ShakeData->Frequency.Min, ShakeData->Frequency.Max);
	
}
