// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DT_PlayerState.generated.h"

class UDT_AttributeComponent;
/**
 * 
 */
UCLASS()
class DISTORTION_API ADT_PlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ADT_PlayerState();

	UDT_AttributeComponent* GetAttributes() const { return Attributes; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDT_AttributeComponent> Attributes;
};
