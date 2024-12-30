// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/DT_ObjectPooledInterface.h"
#include "DT_Projectile.generated.h"

UCLASS()
class DISTORTION_API ADT_Projectile : public AActor, public IDT_ObjectPooledInterface
{
	GENERATED_BODY()
public:	
	ADT_Projectile();
	// virtual void Tick(float DeltaTime) override;

	virtual void OnSpawnFromPool(const FVector_NetQuantize& Location, const FRotator& Rotation) override;
	virtual void OnReturnToPool() override;

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
	float ProjectileSpeed = 3500.0f;
};
