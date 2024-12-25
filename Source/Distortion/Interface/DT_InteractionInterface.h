// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DT_InteractionInterface.generated.h"

UINTERFACE(MinimalAPI)
class UDT_InteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class DISTORTION_API IDT_InteractionInterface
{
	GENERATED_BODY()

public:
	virtual void Interaction(class UDataAsset* Data) = 0;
	virtual void ToAttachSocket(const FName& SocketName) = 0;
};
