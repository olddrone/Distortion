// Fill out your copyright notice in the Description page of Project Settings.


#include "DT_ProjectileGun.h"
#include "Engine/SkeletalMeshSocket.h"
#include "ObjectPool/DT_PoolSubSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"

ADT_ProjectileGun::ADT_ProjectileGun()
{
}

void ADT_ProjectileGun::Attack(const FDamagePacket& DamagePacket, const FVector_NetQuantize& TraceHitTarget)
{
	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	USkeletalMeshComponent* Mesh = Cast<USkeletalMeshComponent>(GetMeshComp());
	const USkeletalMeshSocket* MuzzleFlashSocket = Mesh->GetSocketByName(FName("MuzzleFlash"));

	if (MuzzleFlashSocket)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(Mesh);
		FVector ToTarget = TraceHitTarget - SocketTransform.GetLocation();
		FRotator TargetRotation = ToTarget.Rotation();

		AActor* SpawnedActor = nullptr;
		UDT_PoolSubSystem* PoolSubSystem = GetWorld()->GetSubsystem<UDT_PoolSubSystem>();
		PoolSubSystem->SpawnFromPool(ProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnedActor);

	}
}
