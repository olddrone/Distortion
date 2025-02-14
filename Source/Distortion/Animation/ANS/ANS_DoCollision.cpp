// Fill out your copyright notice in the Description page of Project Settings.

#include "ANS_DoCollision.h"

void UANS_DoCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp)
	{
		CombatInterface = TScriptInterface<IDT_CombatInterface>(MeshComp->GetOwner());
		if (CombatInterface)
			CombatInterface->ActivateCollision(DamagePacket);
	}
}

void UANS_DoCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (MeshComp)
	{
		// 다시 해주어야 문제가 없음
		CombatInterface = TScriptInterface<IDT_CombatInterface>(MeshComp->GetOwner());
		if (CombatInterface)
			CombatInterface->DeactivateCollision();
	}
}
