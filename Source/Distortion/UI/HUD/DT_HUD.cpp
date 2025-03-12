// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_HUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/ViewModel/DT_VM_Attribute.h"
#include "UI/ViewModel/DT_VM_EquipWeapon.h"
#include "UI/ViewModel/DT_VM_TeamScore.h"

void ADT_HUD::InitOverlay(class UDT_AttributeComponent* InAttributes, class UDT_CombatComponent* InCombat)
{
	AttributeVM = NewObject<UDT_VM_Attribute>(this, AttributeVMClass);
    AttributeVM->Init(InAttributes);

	EquipWeaponVM = NewObject<UDT_VM_EquipWeapon>(this, EquipWeaponVMClass);
	EquipWeaponVM->Init(InCombat);

	TeamScoreVM = NewObject<UDT_VM_TeamScore>(this, TeamScoreVMClass);
	TeamScoreVM->Init();

	OverlayWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), OverlayWidgetClass);
    OverlayWidget->AddToViewport();
}

void ADT_HUD::DrawHUD()
{
    Super::DrawHUD();

	FVector2D ViewportSize;
	if (GEngine && bIsActive)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		const FVector2D ViewportCenter(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
		float SpreadScaled = HUDPackage.Spread;

		if (HUDPackage != nullptr)
		{
			DrawCrosshair(HUDPackage.Center, ViewportCenter, FVector2D(0,0));

			DrawCrosshair(HUDPackage.Left, ViewportCenter, FVector2D(-SpreadScaled, 0));
			DrawCrosshair(HUDPackage.Right, ViewportCenter, FVector2D(SpreadScaled, 0));
			DrawCrosshair(HUDPackage.Top, ViewportCenter, FVector2D(0, -SpreadScaled));
			DrawCrosshair(HUDPackage.Bottom, ViewportCenter, FVector2D(0, SpreadScaled));
		}
	}
	
}

void ADT_HUD::BindingWeaponVM()
{
	EquipWeaponVM->BindAmmo();
	EquipWeaponVM->BindAmmoVisibility();
}

void ADT_HUD::DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();
	const FVector2D TextureDrawPoint(ViewportCenter.X - (TextureWidth / 2.f), ViewportCenter.Y - (TextureHeight / 2.f));
	const float ScreenX = ViewportCenter.X - (TextureWidth / 2.f) + Spread.X;
	const float ScreenY = ViewportCenter.Y - (TextureHeight / 2.f) + Spread.Y;
		
	DrawTexture(Texture, ScreenX, ScreenY, TextureWidth, TextureHeight, 0, 0, 1, 1, FLinearColor::Green);
}

void ADT_HUD::GameEndOverlaySet()
{
	OverlayWidget->RemoveFromViewport();
	ToggleCrosshair(false);
	GameEndOverlay->AddToViewport();
}
