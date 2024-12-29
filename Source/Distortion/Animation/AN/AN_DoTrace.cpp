// Fill out your copyright notice in the Description page of Project Settings.

#include "AN_DoTrace.h"
#include "Interface/DT_CombatInterface.h"

void UAN_DoTrace::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IDT_CombatInterface* Interface = Cast<IDT_CombatInterface>(MeshComp->GetOwner());
		if (Interface)
			Interface->ActivateCollision(DamagePacket);
	}
}
