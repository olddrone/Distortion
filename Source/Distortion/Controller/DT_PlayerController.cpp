// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "Character/DT_PlayerCharacter.h"
#include "Camera/DT_CameraManager.h"
#include "UObject/ConstructorHelpers.h"

ADT_PlayerController::ADT_PlayerController()
{
	static ConstructorHelpers::FObjectFinder<UPDA_Input> InputRef(
		TEXT("/Game/Input/PDA_Input.PDA_Input"));
	if (InputRef.Succeeded())
		InputData = InputRef.Object;
	
	PlayerCameraManagerClass = ADT_CameraManager::StaticClass();
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

	EIC->BindAction(InputData->ReloadAction, ETriggerEvent::Started, this, &ADT_PlayerController::Reload);

}

void ADT_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	Init(); // ���� ����
}

void ADT_PlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
	Init(); // Ŭ��
}

void ADT_PlayerController::Init()
{
	EnableInput(this);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
		Subsystem->AddMappingContext(InputData->MappingContext, 0);

	PlayerCharacter = Cast<ADT_PlayerCharacter>(GetCharacter());
	if (IsValid(PlayerCharacter))
	{
		StateInterface = TScriptInterface<IDT_StateInterface>(PlayerCharacter);
	}
}

void ADT_PlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	PlayerCharacter->AddMovementInput(ForwardDirection, InputAxisVector.Y);
	PlayerCharacter->AddMovementInput(RightDirection, InputAxisVector.X);
}

void ADT_PlayerController::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	PlayerCharacter->AddControllerYawInput(LookAxisVector.X);
	PlayerCharacter->AddControllerPitchInput(LookAxisVector.Y);
}

void ADT_PlayerController::Jump()
{
	PlayerCharacter->Jump();
}

void ADT_PlayerController::Dodge()
{
	PlayerCharacter->Dodge();
}

void ADT_PlayerController::Crouch()
{
	(PlayerCharacter->bIsCrouched) ? PlayerCharacter->UnCrouch() : PlayerCharacter->Crouch();	
}

void ADT_PlayerController::RMBStart()
{
	PlayerCharacter->RMB(true);
}

void ADT_PlayerController::RMBEnd()
{
	PlayerCharacter->RMB(false);
}

void ADT_PlayerController::LMBStart()
{
	PlayerCharacter->LMB(true);
}

void ADT_PlayerController::LMBEnd()
{
	PlayerCharacter->LMB(false);
}

void ADT_PlayerController::Equip()
{
	PlayerCharacter->Equip();
}

void ADT_PlayerController::Reload()
{
	PlayerCharacter->Reload();
}