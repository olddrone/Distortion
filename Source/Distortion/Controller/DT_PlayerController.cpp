// Fill out your copyright notice in the Description page of Project Settings.


#include "DT_PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"

ADT_PlayerController::ADT_PlayerController()
{
	bReplicates = true;
}

void ADT_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
		Subsystem->AddMappingContext(InputData->MappingContext, 0);

	ControlledCharacter = GetCharacter();
	if (IsValid(ControlledCharacter))
		StateInterface = Cast<IDT_StateInterface>(ControlledCharacter);
}

void ADT_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(InputComponent);

	EIC->BindAction(InputData->MoveAction, ETriggerEvent::Triggered, this, &ADT_PlayerController::Move);
	EIC->BindAction(InputData->LookAction, ETriggerEvent::Triggered, this, &ADT_PlayerController::Look);
	EIC->BindAction(InputData->JumpAction, ETriggerEvent::Triggered, this, &ADT_PlayerController::Jump);
	EIC->BindAction(InputData->DodgeAction, ETriggerEvent::Triggered, this, &ADT_PlayerController::Dodge);
	EIC->BindAction(InputData->CrouchAction, ETriggerEvent::Triggered, this, &ADT_PlayerController::Crouch);

	EIC->BindAction(InputData->RMBAction, ETriggerEvent::Started, this, &ADT_PlayerController::RMBStart);
	EIC->BindAction(InputData->RMBAction, ETriggerEvent::Completed, this, &ADT_PlayerController::RMBEnd);

	EIC->BindAction(InputData->LMBAction, ETriggerEvent::Started, this, &ADT_PlayerController::LMBStart);
	EIC->BindAction(InputData->LMBAction, ETriggerEvent::Completed, this, &ADT_PlayerController::LMBEnd);

	EIC->BindAction(InputData->EquipAction, ETriggerEvent::Started, this, &ADT_PlayerController::Equip);
}

void ADT_PlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	ControlledCharacter->AddMovementInput(ForwardDirection, InputAxisVector.Y);
	ControlledCharacter->AddMovementInput(RightDirection, InputAxisVector.X);
}

void ADT_PlayerController::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	ControlledCharacter->AddControllerYawInput(LookAxisVector.X);
	ControlledCharacter->AddControllerPitchInput(LookAxisVector.Y);
}

void ADT_PlayerController::Jump()
{
	ControlledCharacter->Jump();
}

void ADT_PlayerController::Dodge()
{
	if (DodgeDelegate.IsBound())
		DodgeDelegate.Execute();
}

void ADT_PlayerController::Crouch()
{
	if (ControlledCharacter->bIsCrouched)
		ControlledCharacter->UnCrouch();
	else
		ControlledCharacter->Crouch();
}

void ADT_PlayerController::RMBStart()
{
	if (RMBDelegate.IsBound())
		RMBDelegate.Execute(true);
}

void ADT_PlayerController::RMBEnd()
{
	if (RMBDelegate.IsBound())
		RMBDelegate.Execute(false);
}

void ADT_PlayerController::LMBStart()
{
	if (LMBDelegate.IsBound())
		LMBDelegate.Execute(true);
}

void ADT_PlayerController::LMBEnd()
{
	if (LMBDelegate.IsBound())
		LMBDelegate.Execute(false);
}

void ADT_PlayerController::Equip()
{
	if (EquipDelegate.IsBound())
		EquipDelegate.Execute();
}
