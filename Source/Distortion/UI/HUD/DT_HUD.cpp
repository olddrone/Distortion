// Fill out your copyright notice in the Description page of Project Settings.


#include "DT_HUD.h"
#include "UI/ViewModel/DT_VM_Attribute.h"
#include "Blueprint/UserWidget.h"

void ADT_HUD::InitOverlay(ADT_PlayerState* InPlayerState, UDT_AttributeComponent* InAttributes)
{
    UE_LOG(LogTemp, Warning, TEXT("InitOverlay"));
    APlayerController* PlayerController = GetOwningPlayerController();

    AttributesViewModel = NewObject<UDT_VM_Attribute>(this, AttributesViewModelClass);
    AttributesViewModel->Init(PlayerController, InPlayerState, InAttributes);

    UUserWidget* OverlayWidget = CreateWidget<UUserWidget>(PlayerController, OverlayWidgetClass);
    OverlayWidget->AddToViewport();
}
