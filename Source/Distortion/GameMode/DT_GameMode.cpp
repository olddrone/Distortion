// Fill out your copyright notice in the Description page of Project Settings.


#include "DT_GameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"

#include "GameState/DT_GameState.h"
#include "PlayerState/DT_PlayerState.h"
#include "Controller/DT_PlayerController.h"

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

void ADT_GameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	ADT_GameState* GS = Cast<ADT_GameState>(UGameplayStatics::GetGameState(this));
	ADT_PlayerState* PS = Exiting->GetPlayerState<ADT_PlayerState>();

	if (GS && PS)
	{
		if (GS->RedTeam.Contains(PS))
			GS->RedTeam.Remove(PS);

		if (GS->BlueTeam.Contains(PS))
			GS->BlueTeam.Remove(PS);
	}
}

void ADT_GameMode::OnPlayerKill(AController* Controller)
{
	ADT_GameState* GS = Cast<ADT_GameState>(UGameplayStatics::GetGameState(this));
	ADT_PlayerState* PS = Controller->GetPlayerState<ADT_PlayerState>();

	if (PS->GetTeam() == ETeam::ET_RedTeam)
		GS->BlueTeamInc();
	else
		GS->RedTeamInc();
}

void ADT_GameMode::MatchWin(ETeam WinningTeam)
{
	UE_LOG(LogTemp, Warning, TEXT("%s Team Win"), 
		WinningTeam == ETeam::ET_BlueTeam ? TEXT("Blue") : TEXT("Red"));
	SetAllPlayerControllersInputMode();
}

void ADT_GameMode::SetAllPlayerControllersInputMode()
{
	for (FConstPlayerControllerIterator Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		// 서버에서 클라이언트 RPC를 보내주어야 로컬 머신에서 정상적 동작
		ADT_PlayerController* PC = Cast<ADT_PlayerController>(Iter->Get());
		if (PC)
			PC->ClientRPCSetInputMode();
	}
}
