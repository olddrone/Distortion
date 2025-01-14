// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_AttributeComponent.h"
#include "Net/UnrealNetwork.h"

UDT_AttributeComponent::UDT_AttributeComponent()
{
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);

	SetHealth(Attributes.MaxHealth);
	SetStamina(Attributes.MaxHealth);
}

void UDT_AttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDT_AttributeComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UDT_AttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDT_AttributeComponent, Health);
}

float UDT_AttributeComponent::ApplyDamage(const float Damage)
{
	SetHealth(Health - Damage);

	UE_LOG(LogTemp, Warning, TEXT("Health : %f"), Health);
	return Damage;
}

void UDT_AttributeComponent::SetHealth(const float InHealth)
{
	Health = FMath::Clamp<float>(InHealth, 0.0f, Attributes.MaxHealth);

	HealthChange.Broadcast(Health);
}

void UDT_AttributeComponent::OnRep_Health()
{
	HealthChange.Broadcast(Health);
	if (Health <= KINDA_SMALL_NUMBER)
		Dead.Broadcast();
}

