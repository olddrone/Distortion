// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/DT_DamageData.h"
#include "Interface/DT_MeshInterface.h"
#include "DT_CollisionManager.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DISTORTION_API UDT_CollisionManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDT_CollisionManager();
	void DoCollision(AActor* Caller);
	void StopCollision();

	void TraceCheck();

	void SetActorsToIgnore(AActor* Actor) { ActorsToIgnore.Add(Actor); }

	void DoLineTrace(const FVector& StartLocation, const FVector& EndLocation,
		TArray<FHitResult>& HitResults, const FColor& Color);
	
	void DoSphereTrace(const FVector& Location, TArray<FHitResult>& HitResults, const FColor& Color);

public:
	FORCEINLINE void SetDamagePacket(const FDamagePacket& InPacket) { DamagePacket = InPacket; }
	FORCEINLINE void SetOwner(APawn* InPawn) { Pawn = InPawn; }
	FORCEINLINE void SetDamage(const float InDamage) { Damage = InDamage; }

private:
	void HandleHitResults(const TArray<FHitResult>& HitResults);

	void DoDamage(const FHitResult& Victim);

	TPair<FVector, FVector> CalculateControlPoints(const FVector& PreStart, const FVector& CurStart, 
		const FVector& PreEnd, const FVector& CurEnd);

	void PerformInterpolatedTraces(const FVector& PreStart, const FVector& CurStart,
		const FVector& PreEnd, const FVector& CurEnd, TPair<FVector, FVector>& CtrlPoint,
		TArray<FHitResult>& HitResults);

private:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> HitActors;
	UPROPERTY()
	TArray<TObjectPtr<AActor>> ActorsToIgnore;
	FTimerHandle Handle;
	const float InRate = 0.034f;		// 1frame == 0.017f
	FDamagePacket DamagePacket;

	TPair<FVector, FVector> BeforePoints = { FVector::ZeroVector,FVector::ZeroVector };
	
	TObjectPtr<APawn> Pawn;
	// TScriptInterface<class ILT_MeshInterface> MeshInterface;
	IDT_MeshInterface* MeshInterface;

	float Damage = 10.f;
};
