// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Data/DT_StateSet.h"
#include "Interface/DT_RespawnInterface.h"
#include "DT_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class DISTORTION_API ADT_GameMode : public AGameMode, public IDT_RespawnInterface
{
	GENERATED_BODY()

public:
	ADT_GameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void RequestPlayerRespawn(ACharacter* Character, AController* Controller) override;
	virtual void Logout(AController* Exiting) override;
	virtual void OnPlayerKill(AController* Controller) override;

	void MatchWin(ETeam WinningTeam);
	void SetAllPlayerControllersInputMode();
};
