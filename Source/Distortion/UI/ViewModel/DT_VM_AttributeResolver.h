// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "View/MVVMViewModelContextResolver.h"
#include "DT_VM_AttributeResolver.generated.h"

/**
 * 런타임 중 뷰모델을 찾거나 생성을 위한 공유 데이터
 */
UCLASS()
class DISTORTION_API UDT_VM_AttributeResolver : public UMVVMViewModelContextResolver
{
	GENERATED_BODY()
	
public:
	virtual UObject* CreateInstance(const UClass* ExpectedType, const UUserWidget* UserWidget, const UMVVMView* View) const override;
};
