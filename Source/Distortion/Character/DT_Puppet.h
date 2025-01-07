// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/DT_PlayerCharacter.h"
#include "DT_Puppet.generated.h"

/**
 * 
 */
UCLASS()
class DISTORTION_API ADT_Puppet : public ADT_PlayerCharacter
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	void SpawnDefaultWeapon();

	
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UDA_Weapon> TmpWeapon;
};
