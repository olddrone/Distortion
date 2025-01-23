// Fill out your copyright notice in the Description page of Project Settings.


#include "PDA_Input.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"


UPDA_Input::UPDA_Input()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_Ref(
		TEXT("/Game/Input/IMC_Player.IMC_Player"));
	if (IMC_Ref.Succeeded())
		MappingContext = IMC_Ref.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_MoveRef(
		TEXT("/Game/Input/InputAction/IA_Move.IA_Move"));
	if (IA_MoveRef.Succeeded())
		MoveAction = IA_MoveRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_LookRef(
		TEXT("/Game/Input/InputAction/IA_Look.IA_Look"));
	if (IA_LookRef.Succeeded())
		LookAction = IA_LookRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_JumpRef(
		TEXT("/Game/Input/InputAction/IA_Jump.IA_Jump"));
	if (IA_JumpRef.Succeeded())
		JumpAction = IA_JumpRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_CrouchRef(
		TEXT("/Game/Input/InputAction/IA_Crouch.IA_Crouch"));
	if (IA_CrouchRef.Succeeded())
		CrouchAction = IA_CrouchRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_DodgeRef(
		TEXT("/Game/Input/InputAction/IA_Dodge.IA_Dodge"));
	if (IA_DodgeRef.Succeeded())
		DodgeAction = IA_DodgeRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_LMBRef(
		TEXT("/Game/Input/InputAction/IA_LMB.IA_LMB"));
	if (IA_LMBRef.Succeeded())
		LMBAction = IA_LMBRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_RMBRef(
		TEXT("/Game/Input/InputAction/IA_RMB.IA_RMB"));
	if (IA_RMBRef.Succeeded())
		RMBAction = IA_RMBRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_EquipRef(
		TEXT("/Game/Input/InputAction/IA_Equip.IA_Equip"));
	if (IA_EquipRef.Succeeded())
		EquipAction = IA_EquipRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_ReloadRef(
		TEXT("/Game/Input/InputAction/IA_Reload.IA_Reload"));
	if (IA_ReloadRef.Succeeded())
		ReloadAction = IA_ReloadRef.Object;
}
