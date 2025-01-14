// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DT_CustomLibrary.generated.h"


USTRUCT()
struct FData
{
	GENERATED_BODY()
	TPair<float, float> Range;
	FName Name;
};

UCLASS()
class DISTORTION_API UDT_CustomLibrary : public UObject
{
	GENERATED_BODY()
public:
	static float CalculateTheta(const FVector& A, const FVector& B);
	static FName CheckSectionName_8Direction(const float& Theta);
	static FName CheckSectionName_4Direction(const float& Theta);
	static FVector BezierCurve_Quadratic(const FVector& P0, const FVector& P1, const FVector& P2, float T);

	static FName CheckSectionName_Guard(const FName& HitDirection, const uint8& AttackDirection);
};
