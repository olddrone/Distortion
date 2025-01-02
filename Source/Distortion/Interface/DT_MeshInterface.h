// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DT_MeshInterface.generated.h"

UINTERFACE(MinimalAPI)
class UDT_MeshInterface : public UInterface
{
	GENERATED_BODY()
};

class DISTORTION_API IDT_MeshInterface
{
	GENERATED_BODY()

public:
	virtual UMeshComponent* GetMeshComp() const = 0;
	virtual FVector GetSocketLocation(const FName& SocketName) const = 0;

};
