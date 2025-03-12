// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "View/MVVMViewModelContextResolver.h"
#include "DT_VM_TeamScoreResolver.generated.h"

/**
 * 
 */
UCLASS()
class DISTORTION_API UDT_VM_TeamScoreResolver : public UMVVMViewModelContextResolver
{
	GENERATED_BODY()
public:
	virtual UObject* CreateInstance(const UClass* ExpectedType, const UUserWidget* UserWidget, const UMVVMView* View) const override;

};
