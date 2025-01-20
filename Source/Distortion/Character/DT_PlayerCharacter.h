// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/DT_BaseCharacter.h"
#include "DT_PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class DISTORTION_API ADT_PlayerCharacter : public ADT_BaseCharacter
{
	GENERATED_BODY()

public:
	ADT_PlayerCharacter();
	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	virtual void Dead() override;

protected:
	// virtual void BeginPlay() override;
	void InitAttributeComp();

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> CameraComponent;

};
