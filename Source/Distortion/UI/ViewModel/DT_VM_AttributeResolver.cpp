// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_VM_AttributeResolver.h"
#include "Blueprint/UserWidget.h"
#include "UI/HUD/DT_HUD.h"
#include "UI/ViewModel/DT_VM_Attribute.h"

UObject* UDT_VM_AttributeResolver::CreateInstance(const UClass* ExpectedType,
	const UUserWidget* UserWidget, const UMVVMView* View) const
{
	ADT_HUD* Hud = Cast<ADT_HUD>(UserWidget->GetOwningPlayer()->GetHUD());
	return (Hud->AttributesViewModel) ? Hud->AttributesViewModel : nullptr;
}
