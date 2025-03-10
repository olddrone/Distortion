// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Data/DT_DamageData.h"
#include "Interface/DT_CombatInterface.h"
#include "ANS_DoCollision.generated.h"

/**
 * 
 */
UCLASS()
class DISTORTION_API UANS_DoCollision : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FDamagePacket DamagePacket;

	UPROPERTY()
	TScriptInterface<IDT_CombatInterface> CombatInterface;
};
