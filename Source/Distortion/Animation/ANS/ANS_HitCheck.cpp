// Fill out your copyright notice in the Description page of Project Settings.

#include "ANS_HitCheck.h"
#include "Interface/DT_CombatInterface.h"

void UANS_HitCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp)
	{
		IDT_CombatInterface* Interface = Cast<IDT_CombatInterface>(MeshComp->GetOwner());
		if (Interface)
			Interface->AttackStart(DamagePacket);
	}
}

void UANS_HitCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (MeshComp)
	{
		IDT_CombatInterface* Interface = Cast<IDT_CombatInterface>(MeshComp->GetOwner());
		if (Interface)
			Interface->AttackEnd();
	}
}
