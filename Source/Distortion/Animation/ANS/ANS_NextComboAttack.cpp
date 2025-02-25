// Fill out your copyright notice in the Description page of Project Settings.

#include "ANS_NextComboAttack.h"

void UANS_NextComboAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp)
	{
		StateInterface = TScriptInterface<IDT_StateInterface>(MeshComp->GetOwner());
		CombatInterface = TScriptInterface<IDT_CombatInterface>(MeshComp->GetOwner());
	}
}

void UANS_NextComboAttack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (MeshComp)
	{
		if (StateInterface->GetLMBDown())
		{
			StateInterface->SetActionState(EActionState::EAS_Unoccupied);
			StateInterface->RestoreState();

			CombatInterface->DoAttack(SectionName);
		}
	}
}
