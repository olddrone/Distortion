// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Interface/DT_StateInterface.h"
#include "Interface/DT_CombatInterface.h"
#include "ANS_NextComboAttack.generated.h"

/**
 * 
 */
UCLASS()
class DISTORTION_API UANS_NextComboAttack : public UAnimNotifyState
{
	GENERATED_BODY()
public:	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FName SectionName;

	UPROPERTY()
	TScriptInterface<IDT_StateInterface> StateInterface;

	UPROPERTY()
	TScriptInterface<IDT_CombatInterface> CombatInterface;

};
