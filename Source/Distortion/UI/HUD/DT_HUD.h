// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Data/DT_Crosshairs.h"
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

public:
	void InitOverlay(ADT_PlayerState* InPlayerState, UDT_AttributeComponent* InAttributes);

	virtual void DrawHUD() override;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TObjectPtr<UDT_VM_Attribute> AttributesViewModel;
	
	FORCEINLINE void SetHUDPackage(const FCrosshairsTextures& InPackage) { HUDPackage = InPackage; }
	
protected:
	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread);

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDT_VM_Attribute> AttributesViewModelClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> OverlayWidgetClass;
private:
	FCrosshairsTextures HUDPackage;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	float CrosshairSpreadMax = 16.f;
};
