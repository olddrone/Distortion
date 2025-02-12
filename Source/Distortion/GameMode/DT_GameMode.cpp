// Fill out your copyright notice in the Description page of Project Settings.


#include "DT_GameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"

#include "GameState/DT_GameState.h"
#include "PlayerState/DT_PlayerState.h"

ADT_GameMode::ADT_GameMode()
{
	/*
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(
		TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
		DefaultPawnClass = PlayerPawnBPClass.Class;
	
	*/
}

void ADT_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// 나중에 완성되면 인터페이스로
	ADT_GameState* GS = Cast<ADT_GameState>(UGameplayStatics::GetGameState(this));
	ADT_PlayerState* PS = NewPlayer->GetPlayerState<ADT_PlayerState>();

	if (PS && PS->GetTeam() == ETeam::ET_NoTeam)
	{
		if (GS->BlueTeam.Num() >= GS->RedTeam.Num())
		{
			GS->RedTeam.AddUnique(PS);
			PS->SetTeam(ETeam::ET_RedTeam);
		}
		else
		{
			GS->BlueTeam.AddUnique(PS);
			PS->SetTeam(ETeam::ET_BlueTeam);
		}
	}
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
		uint8 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		
		RestartPlayerAtPlayerStart(Controller, PlayerStarts[Selection]);
		
	}
}
