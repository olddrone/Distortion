// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_VM_Attribute.h"
#include "Component/DT_AttributeComponent.h"

void UDT_VM_Attribute::Init(UDT_AttributeComponent* InAttributes)
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
	Attributes->HealthChange.AddUObject(this, &UDT_VM_Attribute::SetHealth);
	Attributes->StaminaChange.AddUObject(this, &UDT_VM_Attribute::SetStamina);

	SetMaxStamina(Attributes->GetMaxStamina());
	SetStamina(Attributes->GetStamina());
}

