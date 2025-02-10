// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_VM_Attribute.h"
#include "Component/DT_AttributeComponent.h"
#include "PlayerState/DT_PlayerState.h"

void UDT_VM_Attribute::Init(APlayerController* InPlayerController,
	ADT_PlayerState* InPlayerState, UDT_AttributeComponent* InAttributes)
{
	Attributes = Cast<UDT_AttributeComponent>(InAttributes);
	BindCallbacks();
}

void UDT_VM_Attribute::BindCallbacks()
{
    if (!ensure(Attributes))
        return;

	SetMaxHealth(Attributes->GetMaxHealth());
	SetHealth(Attributes->GetHealth());
	Attributes->HealthChange.AddUObject(this, &UDT_VM_Attribute::OnHealthChanged);
	Attributes->StaminaChange.AddUObject(this, &UDT_VM_Attribute::OnStaminaChanged);

	SetMaxStamina(Attributes->GetMaxStamina());
	SetStamina(Attributes->GetStamina());
}

void UDT_VM_Attribute::OnHealthChanged(const float InHealth)
{
	SetHealth(InHealth);
}

void UDT_VM_Attribute::OnStaminaChanged(const float InStamina)
{
	SetStamina(InStamina);
}
