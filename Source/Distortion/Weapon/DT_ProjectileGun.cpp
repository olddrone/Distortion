// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_ProjectileGun.h"
#include "Engine/SkeletalMeshSocket.h"
#include "ObjectPool/DT_PoolSubSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

ADT_ProjectileGun::ADT_ProjectileGun()
{
}

void ADT_ProjectileGun::Attack(const FDamagePacket& DamagePacket, const FVector_NetQuantize& TraceHitTarget)
{
	if (!CanFire())
		return;

	const USkeletalMeshComponent* Mesh = Cast<USkeletalMeshComponent>(GetMeshComp());
	const USkeletalMeshSocket* MuzzleFlashSocket = Mesh->GetSocketByName(FName("MuzzleFlash"));

	if (MuzzleFlashSocket)
	{
		const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(Mesh);
		const FVector StartLocation = SocketTransform.GetLocation();

		const FVector RandomOffset = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0, ScatterRadius);
		const FVector ScatterEnd = TraceHitTarget + RandomOffset;
		const FVector ToTarget = (ScatterEnd - StartLocation).GetSafeNormal();
		const FRotator TargetRotation = ToTarget.Rotation();

		AActor* SpawnedActor = nullptr;
		UDT_PoolSubSystem* PoolSubSystem = GetWorld()->GetSubsystem<UDT_PoolSubSystem>();
		PoolSubSystem->SpawnFromPool(ProjectileClass, StartLocation, TargetRotation, SpawnedActor, GetOwner());

		// DrawDebugLine(GetWorld(), StartLocation, StartLocation + TargetRotation.Vector() * 5000, FColor::Orange, true);
	}
}
