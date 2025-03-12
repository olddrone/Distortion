// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_VM_TeamScoreResolver.h"
#include "Blueprint/UserWidget.h"
#include "UI/HUD/DT_HUD.h"
#include "UI/ViewModel/DT_VM_TeamScore.h"

UObject* UDT_VM_TeamScoreResolver::CreateInstance(const UClass* ExpectedType, const UUserWidget* UserWidget, const UMVVMView* View) const
{
	ADT_HUD* Hud = Cast<ADT_HUD>(UserWidget->GetOwningPlayer()->GetHUD());
	return (Hud->GetTeamScoreVM()) ? Hud->GetTeamScoreVM() : nullptr;
}
