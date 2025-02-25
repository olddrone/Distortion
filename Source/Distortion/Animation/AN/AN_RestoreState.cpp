// Fill out your copyright notice in the Description page of Project Settings.

#include "AN_RestoreState.h"
#include "Interface/DT_StateInterface.h"

void UAN_RestoreState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IDT_StateInterface* Interface = Cast<IDT_StateInterface>(MeshComp->GetOwner());
		Interface->SetActionState(ActionState);
		Interface->RestoreState();
	}
}
