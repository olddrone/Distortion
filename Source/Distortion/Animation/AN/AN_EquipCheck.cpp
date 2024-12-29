// Fill out your copyright notice in the Description page of Project Settings.

#include "AN_EquipCheck.h"
#include "Interface/DT_InteractionInterface.h"

void UAN_EquipCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IDT_InteractionInterface* Interface = Cast<IDT_InteractionInterface>(MeshComp->GetOwner());
		if (Interface)
			Interface->ToAttachSocket(SocketName);
	}
}
