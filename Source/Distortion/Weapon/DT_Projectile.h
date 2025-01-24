// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/DT_DamageData.h"
#include "Interface/DT_ObjectPooledInterface.h"
#include "DT_Projectile.generated.h"

UCLASS()
class DISTORTION_API ADT_Projectile : public AActor, public IDT_ObjectPooledInterface
{
	GENERATED_BODY()
public:	
	ADT_Projectile();

	virtual void OnSpawnFromPool(const FVector_NetQuantize& Location, const FRotator& Rotation) override;
	virtual void OnReturnToPool() override;

	UFUNCTION()
	void ShowTrace();

	UFUNCTION(Server, Unreliable)
	void ServerPRCShowTrace();

	UFUNCTION(NetMulticast, Unreliable)
	void MulitcastRPCShowTrace();


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> CollisionBox;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float ProjectileSpeed = 15000.0f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UParticleSystem> TraceParticle;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UParticleSystemComponent> TraceComponent;

	FTimerHandle Handle;

	FVector StartLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	float Damage = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	FDamagePacket BulletPacket;
};
