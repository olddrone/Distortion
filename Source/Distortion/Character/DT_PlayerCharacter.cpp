// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Controller/DT_PlayerController.h"
#include "DrawDebugHelpers.h"

#include "PlayerState/DT_PlayerState.h"
#include "UI/HUD/DT_HUD.h"
#include "Camera/DT_CameraManager.h"

ADT_PlayerCharacter::ADT_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 300.f;
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
	
}

void ADT_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ADT_PlayerState* State = GetPlayerState<ADT_PlayerState>();
	if (State)
	{
		AttributeComp = State->GetAttributes();
		if (IsLocallyControlled())
		{
			const APlayerController* PlayerController = GetController<APlayerController>();
			if (ADT_HUD* Hud = PlayerController ? PlayerController->GetHUD<ADT_HUD>() : nullptr)
				Hud->InitOverlay(State, AttributeComp);
		}
	}
}

void ADT_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Start = GetActorLocation();
	Start.Z = 5.f;
	const FVector Forward = GetActorForwardVector().GetSafeNormal();
	FVector End = (GetCharacterMovement()->Velocity).GetSafeNormal();
	FVector Tmp = GetLastMovementInputVector().GetSafeNormal();

	DrawDebugDirectionalArrow(GetWorld(), Start, Start + Forward * 100.f, 5.f, FColor::Red, false, -1.f, 0, 2.f);
	DrawDebugDirectionalArrow(GetWorld(), Start, Start + End * 100.f, 5.f, FColor::Green, false, -1.f, 0, 2.f);
	DrawDebugDirectionalArrow(GetWorld(), Start, Start + Tmp * 100.f, 5.f, FColor::Blue, false, -1.f, 0, 2.f);

	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = GetActorRotation().Pitch;
	ControlRotation.Roll = GetActorRotation().Roll;

	FVector ViewDirection = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X).GetSafeNormal();
	DrawDebugDirectionalArrow(GetWorld(), Start, Start + ViewDirection * 100.f, 5.f, FColor::Orange, false, -1.f, 0, 2.f);
}

void ADT_PlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ADT_PlayerState* State = GetPlayerState<ADT_PlayerState>();
	if (State)
	{
		AttributeComp = State->GetAttributes();
		if (IsLocallyControlled())
		{
			const APlayerController* PlayerController = GetController<APlayerController>();
			if (ADT_HUD* Hud = PlayerController ? PlayerController->GetHUD<ADT_HUD>() : nullptr)
				Hud->InitOverlay(State, AttributeComp);
		}
	}
}

void ADT_PlayerCharacter::RMB(bool bHoldRotationYaw)
{
	Super::RMB(bHoldRotationYaw);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController && GetEquipWeaponType() == EWeaponType::EWT_Gun)
	{
		ADT_CameraManager* CameraManager = Cast<ADT_CameraManager>(PlayerController->PlayerCameraManager);
		if (CameraManager)
			CameraManager->SetZoomState(bRMBDown);
	}
}

void ADT_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	auto* controller = Cast<ADT_PlayerController>(GetController());

	if (IsValid(controller))
	{
		controller->DodgeDelegate.BindUObject(this, &ADT_BaseCharacter::Dodge);
		controller->RMBDelegate.BindUObject(this, &ADT_PlayerCharacter::RMB);
		controller->LMBDelegate.BindUObject(this, &ADT_BaseCharacter::LMB);
		controller->EquipDelegate.BindUObject(this, &ADT_BaseCharacter::Equip);
	}
}
