// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_GameState.h"
#include "ObjectPool/DT_PoolSubSystem.h"
#include "Net/UnrealNetwork.h"

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
{	/*
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(
		GetWorld()->GetFirstPlayerController());
	if (BPlayer)
		BPlayer->SetHUDRedTeamScore(RedTeamScore);
	*/
}

void ADT_GameState::OnRep_BlueTeamScore()
{	/*
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(
		GetWorld()->GetFirstPlayerController());
	if (BPlayer)
		BPlayer->SetHUDRedTeamScore(RedTeamScore);
	*/
}

void ADT_GameState::RedTeamInc()
{
	++RedTeamScore;

	/*
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(
		GetWorld()->GetFirstPlayerController());
	if (BPlayer)
		BPlayer->SetHUDRedTeamScore(RedTeamScore);
	*/
}

void ADT_GameState::BlueTeamInc()
{
	++BlueTeamScore;

	/*
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(
		GetWorld()->GetFirstPlayerController());
	if (BPlayer)
		BPlayer->SetHUDBlueTeamScore(BlueTeamScore);
	*/
}