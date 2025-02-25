// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_CameraManager.h"
#include "Camera/DT_LegacyCameraShake.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

ADT_CameraManager::ADT_CameraManager()
{
	HitShake = UDT_LegacyCameraShake::StaticClass();
}

void ADT_CameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	if (Character.IsValid())
	{
		FVector TargetCrouchOffset = FVector(0.f, 0.f, 
			MoveComp->GetCrouchedHalfHeight() - Character->GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		FVector Offset = FMath::Lerp(FVector::ZeroVector, TargetCrouchOffset, FMath::Clamp(CrouchBlendTime / CrouchBlendDuration, 0.f, 1.f));

		if (MoveComp->IsCrouching())
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime + DeltaTime, 0.f, CrouchBlendDuration);
			Offset -= TargetCrouchOffset;
		}
		else
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime - DeltaTime, 0.f, CrouchBlendDuration);

		if (MoveComp->IsMovingOnGround())
			OutVT.POV.Location += Offset;

		const float ChangeFOV = (bIsZooming) ? ZoomedFOV : DefaultFOV;
		InterpFOV = FMath::FInterpTo(InterpFOV, ChangeFOV, DeltaTime, ZoomInterpSpeed);
		SetFOV(InterpFOV);
	}
	else
	{
		Character = Cast<ACharacter>(GetOwningPlayerController()->GetPawn());
		if (Character.IsValid())
			MoveComp = Character->GetCharacterMovement();
	}
}

void ADT_CameraManager::DoHitCameraShake()
{
	GetOwningPlayerController()->ClientStartCameraShake(HitShake);
}