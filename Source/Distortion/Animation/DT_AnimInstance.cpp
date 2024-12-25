// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UDT_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation(); 
	Character = Cast<ACharacter>(TryGetPawnOwner());
	if (IsValid(Character))
	{
		CharacterMovement = Character->GetCharacterMovement();
		StateInterface = Cast<IDT_StateInterface>(Character);
		AOInterface = Cast<IDT_AimOffsetInterface>(Character);
	}
}

void UDT_AnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (Character && CharacterMovement)
	{
		Velocity = CharacterMovement->Velocity;
		bIsFalling = CharacterMovement->IsFalling();
		bIsCrouch = CharacterMovement->IsCrouching();
		bShouldMove = (CharacterMovement->GetCurrentAcceleration() != FVector(0));
		bZoom = StateInterface->GetRMBDown();

		MoveForward = FVector::DotProduct(Character->GetActorForwardVector(), Velocity);
		MoveRight = FVector::DotProduct(Character->GetActorRightVector(), Velocity);
		EquipWeaponType = StateInterface->GetEquipWeaponType();
		TurnInPlace = AOInterface->GetTurnInPlace();

		bRotateRootBone = AOInterface->ShouldRotateRootBone();

		if (EquipWeaponType == EWeaponType::EWT_Gun)
		{
			AO_Yaw = AOInterface->GetAimOffsetYaw();
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
