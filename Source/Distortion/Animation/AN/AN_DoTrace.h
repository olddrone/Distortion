// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Data/DT_DamageData.h"
#include "AN_DoTrace.generated.h"

/**
 * 
 */
UCLASS()
class DISTORTION_API UAN_DoTrace : public UAnimNotify
{
	GENERATED_BODY()

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FDamagePacket DamagePacket;
};
