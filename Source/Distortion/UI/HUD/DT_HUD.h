// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Data/DT_Crosshairs.h"
#include "Interface/DT_HUDInterface.h"
#include "DT_HUD.generated.h"

class UDT_VM_Attribute;
class UDT_VM_EquipWeapon;
/**
 * 
 */
UCLASS()
class DISTORTION_API ADT_HUD : public AHUD, public IDT_HUDInterface
{
	GENERATED_BODY()

public:
	void InitOverlay(class UDT_AttributeComponent* InAttributes, class UDT_CombatComponent* InCombat);

	virtual void DrawHUD() override;

	FORCEINLINE UDT_VM_Attribute* GetAttributeVM() const { return AttributeVM; }
	UDT_VM_EquipWeapon* GetEquipWeaponVM() const { return EquipWeaponVM; }
	virtual void SetHUDPackage(const FCrosshairsTextures& InPackage) override { HUDPackage = InPackage; }
	
	virtual void BindingWeaponVM() override;
	virtual void GameEndOverlaySet() override;

protected:
	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread);

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TObjectPtr<UDT_VM_Attribute> AttributeVM;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDT_VM_Attribute> AttributeVMClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TObjectPtr<UDT_VM_EquipWeapon> EquipWeaponVM;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDT_VM_EquipWeapon> EquipWeaponVMClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> OverlayWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> OverlayWidget;

private:
	FCrosshairsTextures HUDPackage;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> GameEndOverlay;

	bool bIsActive = true;
};
