// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_VM_Attribute.h"
#include "Component/DT_AttributeComponent.h"
#include "PlayerState/DT_PlayerState.h"

void UDT_VM_Attribute::Init(APlayerController* InPlayerController,
	ADT_PlayerState* InPlayerState, UDT_AttributeComponent* InAttributes)
{
	Attributes = Cast<UDT_AttributeComponent>(InAttributes);
	BindCallbacks();
	NotifyInit();
}

void UDT_VM_Attribute::NotifyInit()
{
	ReceiveInit();
}

void UDT_VM_Attribute::BindCallbacks()
{
    if (!ensure(Attributes))
        return;

	SetHealth(Attributes->GetHealth());
	SetMaxHealth(Attributes->GetMaxHealth());
	Attributes->HealthChange.AddUObject(this, &UDT_VM_Attribute::OnHealthChanged);
}

void UDT_VM_Attribute::OnHealthChanged(float NewHealth)
{
	SetHealth(NewHealth);
}