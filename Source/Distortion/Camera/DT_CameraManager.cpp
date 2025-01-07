// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_CameraManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

void ADT_CameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	if (ACharacter* Character = Cast<ACharacter>(GetOwningPlayerController()->GetPawn()))
	{
		UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement();
		FVector TargetCrouchOffset = FVector(0.f, 0.f, MoveComp->GetCrouchedHalfHeight() - Character->GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
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

		///
		const float ChangeFOV = (bIsZooming) ? ZoomedFOV : DefaultFOV;
		TestFOV = FMath::FInterpTo(TestFOV, ChangeFOV, DeltaTime, ZoomInterpSpeed);
		SetFOV(TestFOV);
	}
}