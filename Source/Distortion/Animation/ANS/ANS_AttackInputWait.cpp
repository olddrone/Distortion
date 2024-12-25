// Fill out your copyright notice in the Description page of Project Settings.

#include "ANS_AttackInputWait.h"
#include "Interface/DT_StateInterface.h"

void UANS_AttackInputWait::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp)
	{
		IDT_StateInterface* Interface = Cast<IDT_StateInterface>(MeshComp->GetOwner());
		if (Interface)
			Interface->SetLMBDown(false);
	}
}

void UANS_AttackInputWait::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (MeshComp)
	{
		IDT_StateInterface* Interface = Cast<IDT_StateInterface>(MeshComp->GetOwner());
		if (Interface)
			Interface->SetLMBDown(false);
	}
}
