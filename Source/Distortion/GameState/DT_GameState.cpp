// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_GameState.h"
#include "ObjectPool/DT_PoolSubSystem.h"
#include "Net/UnrealNetwork.h"
#include "GameMode/DT_GameMode.h"
#include "Data/DT_StateSet.h"

void ADT_GameState::BeginPlay()
{
	Super::BeginPlay();

	UDT_PoolSubSystem* PoolSubSystem = GetWorld()->GetSubsystem<UDT_PoolSubSystem>();
	PoolSubSystem->InitializePool(PoolActor, PoolSize);
}

void ADT_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADT_GameState, RedTeamScore);
	DOREPLIFETIME(ADT_GameState, BlueTeamScore);
}

void ADT_GameState::OnRep_RedTeamScore()
{
	RedTeamScoreChange.ExecuteIfBound(RedTeamScore);
}

void ADT_GameState::OnRep_BlueTeamScore()
{
	BlueTeamScoreChange.ExecuteIfBound(BlueTeamScore);
}

void ADT_GameState::RedTeamInc()
{
	++RedTeamScore;
	RedTeamScoreChange.ExecuteIfBound(RedTeamScore);

	if (RedTeamScore >= WinScore)
	{
		ADT_GameMode* GM = Cast<ADT_GameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
			GM->MatchWin(ETeam::ET_RedTeam);

	}
}

void ADT_GameState::BlueTeamInc()
{
	++BlueTeamScore;
	BlueTeamScoreChange.ExecuteIfBound(BlueTeamScore);

	if (BlueTeamScore >= WinScore)
	{	
		ADT_GameMode* GM = Cast<ADT_GameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
			GM->MatchWin(ETeam::ET_BlueTeam);
	}
}