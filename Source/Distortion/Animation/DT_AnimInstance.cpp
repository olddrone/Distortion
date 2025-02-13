// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UDT_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation(); 
	Character = Cast<ACharacter>(TryGetPawnOwner());
	if (Character.IsValid())
	{
		CharacterMovement = Character->GetCharacterMovement();
		StateInterface = TScriptInterface<IDT_StateInterface>(Character.Get());
		AOInterface = TScriptInterface<IDT_AimOffsetInterface>(Character.Get());
	}
}

void UDT_AnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (Character.IsValid() && CharacterMovement.IsValid())
	{
		Velocity = CharacterMovement->Velocity;
		bIsFalling = CharacterMovement->IsFalling();
		bIsCrouch = CharacterMovement->IsCrouching();
		bShouldMove = (CharacterMovement->GetCurrentAcceleration() != FVector::ZeroVector);
		bZoom = StateInterface->GetRMBDown();

		MoveForward = FVector::DotProduct(Character->GetActorForwardVector(), Velocity);
		MoveRight = FVector::DotProduct(Character->GetActorRightVector(), Velocity);
		EquipWeaponType = StateInterface->GetEquipWeaponType();

		if (EquipWeaponType == EWeaponType::EWT_Gun)
		{
			SetAOYaw(DeltaTime);
			SetAOPitch();
			SetLeftHandPosition();
		}
	}
}

void UDT_AnimInstance::SetAOPitch()
{
	FRotator CharacterRotation = Character->GetActorRotation();
	FRotator AimRotation = Character->GetBaseAimRotation();
	AO_Pitch = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, CharacterRotation).Pitch;
}

void UDT_AnimInstance::SetLeftHandPosition()
{
	LeftHandTransform = AOInterface->GetWeaponSocketTransform("LeftHandSocket");
	FVector OutPosition;
	FRotator OutRotation;
	Character->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
	LeftHandTransform.SetLocation(OutPosition);
	LeftHandTransform.SetRotation(FQuat(OutRotation));
}

void UDT_AnimInstance::SetAOYaw(const float& DeltaTime)
{
	Character->bUseControllerRotationYaw = true;
	CharacterMovement->bOrientRotationToMovement = false;
	if (UKismetMathLibrary::VSizeXY(Velocity) == 0.f && !bIsFalling)
	{
		bRotateRootBone = true;
		FRotator CurAimRotation = FRotator(0, Character->GetBaseAimRotation().Yaw, 0);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurAimRotation, StartingAimRotation);
		AO_Yaw = DeltaAimRotation.Yaw;

		if (TurnInPlace == ETurnInPlace::ETIP_NotTurn)
			InterpAOYaw = AO_Yaw;

		CheckTurnInPlace(DeltaTime);
	}
	else
	{
		bRotateRootBone = false; 
		StartingAimRotation = FRotator(0.f, Character->GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;

		TurnInPlace = ETurnInPlace::ETIP_NotTurn;
	}
}

void UDT_AnimInstance::CheckTurnInPlace(const float& DeltaTime)
{
	if (AO_Yaw > 90.f)
		TurnInPlace = ETurnInPlace::ETIP_Right;
	else if (AO_Yaw < -90.f)
		TurnInPlace = ETurnInPlace::ETIP_Left;

	if (TurnInPlace != ETurnInPlace::ETIP_NotTurn)
	{
		InterpAOYaw = FMath::FInterpTo(InterpAOYaw, 0.f, DeltaTime, 4.f);
		AO_Yaw = InterpAOYaw;
		if (FMath::Abs(AO_Yaw) < 15.f)
		{
			TurnInPlace = ETurnInPlace::ETIP_NotTurn;
			StartingAimRotation = FRotator(0.f, Character->GetBaseAimRotation().Yaw, 0.f);
		}
	}
}

