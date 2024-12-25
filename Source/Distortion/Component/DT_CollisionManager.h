// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/DT_DamageData.h"
#include "Interface/DT_MeshInterface.h"
#include "Interface/DT_CombatInterface.h"
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

	UFUNCTION(Server, Reliable)
	void ServerRPCDoDamage(const FHitResult& Victim);

	void DoSphereTrace(const FVector& StartLocation, const FVector& EndLocation,
		TArray<FHitResult>& HitResults, const FColor& Color);

public:
	FORCEINLINE void SetCharacter(AActor* InCharacter) { Character = InCharacter; }
	FORCEINLINE void SetSocketName(const FName& InStartSocketName, const FName& InEndSocketName) { StartSocketName = InStartSocketName; EndSocketName = InEndSocketName; }

protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> HitActors;
	UPROPERTY()
	TArray<TObjectPtr<AActor>> ActorsToIgnore;
	FTimerHandle Handle;
	float InRate = 0.025f;		// 1frame == 0.017f
	FDamagePacket DamageInfo;
	FVector BeforeStart, BeforeEnd;

	FName StartSocketName, EndSocketName;

	AActor* Character;
	// TScriptInterface<class ILT_MeshInterface> MeshInterface;
	IDT_MeshInterface* MeshInterface;
};
