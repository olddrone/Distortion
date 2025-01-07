// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DT_GameState.generated.h"

/**
 * 
 */
UCLASS()
class DISTORTION_API ADT_GameState : public AGameState
{
	GENERATED_BODY()
public:
	ADT_GameState();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, meta = (PrivateAllowAccess = "true"))
	TSubclassOf<AActor> PoolActor;

	UPROPERTY(EditAnywhere, meta = (PrivateAllowAccess = "true"))
	uint32 PoolSize;
};
