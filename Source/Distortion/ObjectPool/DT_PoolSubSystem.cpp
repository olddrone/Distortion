// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_PoolSubSystem.h"
#include "Interface/DT_ObjectPooledInterface.h"

void UDT_PoolSubSystem::InitializePool(TSubclassOf<AActor> PoolClass, int32 MaxSize)
{
    FPoolArray& ObjectPool = ObjectPools.FindOrAdd(PoolClass);
    for (int32 i = 0; i < MaxSize; ++i) 
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        AActor* NewActor = GetWorld()->SpawnActor<AActor>(PoolClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
        
        if (NewActor && PoolClass.Get()->ImplementsInterface(UDT_ObjectPooledInterface::StaticClass())) 
        {
            IDT_ObjectPooledInterface* Interface = Cast<IDT_ObjectPooledInterface>(NewActor);
            Interface->OnReturnToPool();

            NewActor->SetActorHiddenInGame(true);
            NewActor->SetActorEnableCollision(false);
            ObjectPool.Add(NewActor);
        }
    }
}

void UDT_PoolSubSystem::SpawnFromPool(TSubclassOf<AActor> PoolClass, const FVector_NetQuantize& Location, const FRotator& Rotation, AActor*& SpawnedActor)
{
    
    SpawnedActor = GetActorFromPool(PoolClass, Location, Rotation);
}

void UDT_PoolSubSystem::ReturnToPool(AActor* Poolable)
{
    if (!Poolable) return;

    UClass* ActorClass = Poolable->GetClass();

    if (ActorClass->ImplementsInterface(UDT_ObjectPooledInterface::StaticClass()))
    {
        IDT_ObjectPooledInterface* Interface = Cast<IDT_ObjectPooledInterface>(Poolable);
        Interface->OnReturnToPool();
        Poolable->SetActorHiddenInGame(true);
        Poolable->SetActorEnableCollision(false);
        FPoolArray& ObjectPool = ObjectPools.FindOrAdd(ActorClass);
        ObjectPool.Add(Poolable);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Actor [%s] Destroy"), *Poolable->GetName());
        Poolable->Destroy();
    }
}

AActor* UDT_PoolSubSystem::GetActorFromPool(TSubclassOf<AActor> PoolClass, const FVector_NetQuantize& Location, const FRotator& Rotation)
{
    FPoolArray& ObjectPool = ObjectPools.FindOrAdd(PoolClass);
    if (!ObjectPool.IsEmpty())
    {
        AActor* Actor = ObjectPool.Pop();
        if (Actor) 
        {
            Actor->SetActorLocationAndRotation(Location, Rotation);
            Actor->SetActorHiddenInGame(false);

            IDT_ObjectPooledInterface* Interface = Cast<IDT_ObjectPooledInterface>(Actor);
            Interface->OnSpawnFromPool(Location, Rotation);
            return Actor;
        }
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    AActor* NewActor = GetWorld()->SpawnActor<AActor>(PoolClass, Location, Rotation, SpawnParams);

    if (NewActor && PoolClass.Get()->ImplementsInterface(UDT_ObjectPooledInterface::StaticClass())) 
    {
        IDT_ObjectPooledInterface* Interface = Cast<IDT_ObjectPooledInterface>(NewActor);
        Interface->OnSpawnFromPool(Location, Rotation);
    }

    return NewActor;
}
