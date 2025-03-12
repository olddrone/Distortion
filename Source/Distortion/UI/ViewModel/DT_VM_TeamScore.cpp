// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_VM_TeamScore.h"
#include "Kismet/GameplayStatics.h"
#include "GameState/DT_GameState.h"

void UDT_VM_TeamScore::Init()
{
	BindCallbacks();
}

void UDT_VM_TeamScore::BindCallbacks()
{
	ADT_GameState* GS = Cast<ADT_GameState>(UGameplayStatics::GetGameState(this));
	
	SetBlueTeamScore(GS->GetBlueTeamScore());
	SetRedTeamScore(GS->GetRedTeamScore());
	GS->RedTeamScoreChange.BindUObject(this, &UDT_VM_TeamScore::SetRedTeamScore);
	GS->BlueTeamScoreChange.BindUObject(this, &UDT_VM_TeamScore::SetBlueTeamScore);
}
