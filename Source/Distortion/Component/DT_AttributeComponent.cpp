// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_AttributeComponent.h"
#include "Net/UnrealNetwork.h"

UDT_AttributeComponent::UDT_AttributeComponent()
{
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
}

void UDT_AttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDT_AttributeComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetHealth(Attributes.MaxHealth);
}

void UDT_AttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDT_AttributeComponent, Health);
}

float UDT_AttributeComponent::ApplyDamage(const float Damage)
{
	SetHealth(Health - Damage);

	if (Health <= 0.f)
		Dead.Broadcast();
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
	if (Health <= 0.f)
		Dead.Broadcast();
}
