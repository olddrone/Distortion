// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_PlayerState.h"
#include "Component/DT_AttributeComponent.h"

ADT_PlayerState::ADT_PlayerState()
{
	Attributes = CreateDefaultSubobject<UDT_AttributeComponent>("Attributes");
	NetUpdateFrequency = 60.f;
}
