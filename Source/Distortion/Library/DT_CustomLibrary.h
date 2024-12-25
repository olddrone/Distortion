// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DT_CustomLibrary.generated.h"

#define EPSILON 0.0001f
#define SPHERE_TRACE(SSocket, ESocket, Debug) UKismetSystemLibrary::SphereTraceSingle(GetWorld(), SSocket, ESocket, 15.f, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), false, ActorsToIgnore, Debug, OutHit, true, FColor::Orange, FColor::Green, 0.5f);

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
	
};
