// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Data/DT_StateSet.h"
#include "Interface/DT_StateInterface.h"
#include "AN_RestoreState.generated.h"

/**
 * 
 */
UCLASS()
class DISTORTION_API UAN_RestoreState : public UAnimNotify
{
	GENERATED_BODY()
protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState;
};