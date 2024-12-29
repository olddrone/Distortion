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
	virtual void Tick(float DeltaTime) override;

	virtual void OnSpawnFromPool_Implementation() override;
	virtual void OnReturnToPool_Implementation() override;

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> CollisionBox;

};
