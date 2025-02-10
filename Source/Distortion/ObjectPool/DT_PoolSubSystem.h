// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DT_PoolSubSystem.generated.h"

USTRUCT()
struct FPoolArray {
    GENERATED_BODY()

    UPROPERTY()
    TArray<TObjectPtr<AActor>> ObjectPool;

    bool IsEmpty() { return ObjectPool.IsEmpty(); }
    AActor* Pop() { return ObjectPool.Pop(); }
    void Add(AActor* ActorToAdd) { ObjectPool.Add(ActorToAdd); }
};

/**
 * 
 */
UCLASS()
class DISTORTION_API UDT_PoolSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
    UFUNCTION()
    void InitializePool(TSubclassOf<AActor> PoolClass, int32 MaxSize);

    UFUNCTION()
    void SpawnFromPool(TSubclassOf<AActor> PoolClass, const FVector_NetQuantize& Location, 
        const FRotator& Rotation, AActor* SpawnedActor, AActor* Owner);
    
    UFUNCTION()
    void ReturnToPool(AActor* Poolable);

private:
    AActor* GetActorFromPool(TSubclassOf<AActor> PoolClass, 
        const FVector_NetQuantize& Location, const FRotator& Rotation, AActor* Owner);

private:
    UPROPERTY()
    TMap<UClass*, FPoolArray> ObjectPools;
};
