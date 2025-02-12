// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_HUD.h"
#include "UI/ViewModel/DT_VM_Attribute.h"
#include "UI/ViewModel/DT_VM_EquipWeapon.h"
#include "Blueprint/UserWidget.h"

void ADT_HUD::InitOverlay(class UDT_AttributeComponent* InAttributes, class UDT_CombatComponent* InCombat)
{
	AttributeVM = NewObject<UDT_VM_Attribute>(this, AttributeVMClass);
    AttributeVM->Init(InAttributes);

	EquipWeaponVM = NewObject<UDT_VM_EquipWeapon>(this, EquipWeaponVMClass);
	EquipWeaponVM->Init(InCombat);

    UUserWidget* OverlayWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), OverlayWidgetClass);
    OverlayWidget->AddToViewport();
}

void ADT_HUD::DrawHUD()
{
    Super::DrawHUD();

	FVector2D ViewportSize;
	if (GEngine)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		const FVector2D ViewportCenter(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
		float SpreadScaled = CrosshairSpreadMax * HUDPackage.Spread;

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

void ADT_HUD::BindingEquipVM()
{
	EquipWeaponVM->BindAmmo();
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