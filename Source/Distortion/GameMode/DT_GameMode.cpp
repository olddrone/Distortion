// Fill out your copyright notice in the Description page of Project Settings.


#include "DT_GameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"

#include "Character/DT_PlayerCharacter.h"
ADT_GameMode::ADT_GameMode()
{
	/*
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(
		TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
		DefaultPawnClass = PlayerPawnBPClass.Class;
	
	*/
}

void ADT_GameMode::RequestPlayerRespawn(ACharacter* Character, AController* Controller)
{
	if (Character)
	{
		Character->Reset();
		Character->Destroy();
	}
	if (Controller)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		
		RestartPlayerAtPlayerStart(Controller, PlayerStarts[Selection]);
		
	}
}
