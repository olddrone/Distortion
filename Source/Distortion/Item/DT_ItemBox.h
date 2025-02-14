// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/DA_Weapon.h"
#include "Interface/DT_InteractionInterface.h"
#include "DT_ItemBox.generated.h"

class USphereComponent;

UCLASS()
class DISTORTION_API ADT_ItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	ADT_ItemBox();

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FORCEINLINE USphereComponent* GetOverlapComponent() const { return OverlapComp; }

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> OverlapComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class UStaticMeshComponent> MeshComp;

	UPROPERTY(EditAnywhere, Category = "Data")
	TObjectPtr<UDA_Weapon> WeaponData;

	UPROPERTY()
	TScriptInterface<IDT_InteractionInterface> ActorInterface;
};
