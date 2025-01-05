// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DT_HUD.generated.h"

class ADT_PlayerState;
class UDT_AttributeComponent;
class UDT_VM_Attribute;
/**
 * 
 */
UCLASS()
class DISTORTION_API ADT_HUD : public AHUD
{
	GENERATED_BODY()
public:
	void InitOverlay(ADT_PlayerState* InPlayerState, UDT_AttributeComponent* InAttributes);

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TObjectPtr<UDT_VM_Attribute> AttributesViewModel;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDT_VM_Attribute> AttributesViewModelClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> OverlayWidgetClass;
};
