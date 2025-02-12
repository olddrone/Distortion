// Fill out your copyright notice in the Description page of Project Settings.


#include "DT_LegacyCameraShake.h"

UDT_LegacyCameraShake::UDT_LegacyCameraShake()
{
	// 이것도 데이터에셋으로
	OscillationDuration = 0.25f;
	OscillationBlendInTime = 0.05f;
	OscillationBlendOutTime = 0.05f;

	RotOscillation.Pitch.Amplitude = FMath::RandRange(3.f, 5.f);
	RotOscillation.Pitch.Frequency = FMath::RandRange(15.f, 20.f);

	RotOscillation.Yaw.Amplitude = FMath::RandRange(3.f, 5.f);
	RotOscillation.Yaw.Frequency = FMath::RandRange(15.f, 20.f);
}
