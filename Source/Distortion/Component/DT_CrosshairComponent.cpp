// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_CrosshairComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/HUD.h"
#include "Data/DT_Crosshairs.h"
#include "Interface/DT_CombatInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interface/DT_HUDInterface.h"
#include "Interface/DT_StateInterface.h"
#include "Component/DT_CombatComponent.h"
#include "Weapon/DT_BaseWeapon.h"

UDT_CrosshairComponent::UDT_CrosshairComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(false);
}

void UDT_CrosshairComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACharacter>(GetOwner());
	if (Character.IsValid())
	{
		Controller = Cast<APlayerController>(Character->GetController());
		if (Controller.IsValid())
		{
			IDT_CombatInterface* CombatInterface = Cast<IDT_CombatInterface>(Character.Get());
			CombatComponent = CombatInterface->GetCombatComponent();

			HUDInterface = TScriptInterface<IDT_HUDInterface>(Controller->GetHUD());
			StateInterface = TScriptInterface<IDT_StateInterface>(Character.Get());
		}
	}
}

void UDT_CrosshairComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (CombatComponent && CombatComponent->GetEquipWeapon())
		SetHUDCrosshairs(DeltaTime);
}

void UDT_CrosshairComponent::SetHUDCrosshairs(float DeltaTime)
{
	FCrosshairsTextures Textures;
	Textures = CombatComponent->GetWeapon()->GetCrosshairs();
	FVector2D WalkSpeedRange(0, Character->GetCharacterMovement()->MaxWalkSpeed);
	FVector2D VelocityMultiplierRange(0, 1);

	TPair<float, float> InAirValue = (Character->GetCharacterMovement()->IsFalling())
		? TPair<float, float>(2.25f, 2.25f)
		: TPair<float, float>(0.f, 30.f);

	CrosshairFactor.Velocity = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, UKismetMathLibrary::VSizeXY(Character->GetVelocity()));
	CrosshairFactor.InAir = FMath::FInterpTo(CrosshairFactor.InAir, InAirValue.Key, DeltaTime, InAirValue.Value);
	CrosshairFactor.Aim = FMath::FInterpTo(CrosshairFactor.Aim, CrosshairFactor.Zoom, DeltaTime, 30.f);
	CrosshairFactor.Shooting = FMath::FInterpTo(CrosshairFactor.Shooting, 0.f, DeltaTime, 40.f);

	Spread = 0.5f + CrosshairFactor.Velocity + CrosshairFactor.InAir - CrosshairFactor.Aim + CrosshairFactor.Shooting;
	if (!StateInterface->GetRMBDown())
		Spread *= SpreadMax;
	Textures.Spread = Spread;
	HUDInterface->SetHUDPackage(Textures);

}