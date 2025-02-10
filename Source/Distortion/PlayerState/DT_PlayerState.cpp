// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_PlayerState.h"
#include "Component/DT_AttributeComponent.h"
#include "Net/UnrealNetwork.h"
#include "Interface/DT_StateInterface.h"

ADT_PlayerState::ADT_PlayerState()
{
	Attributes = CreateDefaultSubobject<UDT_AttributeComponent>("Attributes");
	NetUpdateFrequency = 60.f;
}

void ADT_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADT_PlayerState, Team);
}

void ADT_PlayerState::SetTeam(const ETeam& TeamToSet)
{
	Team = TeamToSet;
	IDT_StateInterface* Interface = Cast<IDT_StateInterface>(GetPawn());
	if (Interface)
		Interface->SetTeamColor(Team);
}

void ADT_PlayerState::OnRep_Team()
{
    IDT_StateInterface* Interface = Cast<IDT_StateInterface>(GetPawn());
    if (Interface)
        Interface->SetTeamColor(Team);
}
