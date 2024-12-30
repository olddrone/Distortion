// Fill out your copyright notice in the Description page of Project Settings.

#include "GameState/DT_GameState.h"
#include "ObjectPool/DT_PoolSubSystem.h"


void ADT_GameState::BeginPlay()
{
	Super::BeginPlay();

	UDT_PoolSubSystem* PoolSubSystem = GetWorld()->GetSubsystem<UDT_PoolSubSystem>();
	PoolSubSystem->InitializePool(PoolActor, PoolSize);

}
