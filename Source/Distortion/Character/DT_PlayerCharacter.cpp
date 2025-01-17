// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Controller/DT_PlayerController.h"

#include "PlayerState/DT_PlayerState.h"
#include "UI/HUD/DT_HUD.h"
#include "Camera/DT_CameraManager.h"
#include "Component/DT_AttributeComponent.h"

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

	// 리슨 서버
	InitAttributeComp();
}

void ADT_PlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 클라
	InitAttributeComp();
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

	if (IsValid(controller) && IsLocallyControlled())
	{
		controller->DodgeDelegate.BindUObject(this, &ADT_BaseCharacter::Dodge);
		controller->RMBDelegate.BindUObject(this, &ADT_PlayerCharacter::RMB);
		controller->LMBDelegate.BindUObject(this, &ADT_BaseCharacter::LMB);
		controller->EquipDelegate.BindUObject(this, &ADT_BaseCharacter::Equip);
	}
}

void ADT_PlayerCharacter::InitAttributeComp()
{
	ADT_PlayerState* State = GetPlayerState<ADT_PlayerState>();
	if (State)
	{
		// AttributeComp = State->GetAttributes();
		// AttributeComp->Dead.AddUObject(this, &ADT_PlayerCharacter::Dead);

		const APlayerController* PlayerController = GetController<APlayerController>();
		ADT_HUD* Hud = (PlayerController) ? PlayerController->GetHUD<ADT_HUD>() : nullptr;
		if (IsValid(Hud))
			Hud->InitOverlay(State, AttributeComp);
	}
}

void ADT_PlayerCharacter::Dead()
{
	Super::Dead();
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
		DisableInput(PlayerController);
}