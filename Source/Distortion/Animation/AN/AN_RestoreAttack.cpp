// Fill out your copyright notice in the Description page of Project Settings.

#include "AN_RestoreAttack.h"
#include "Interface/DT_StateInterface.h"
#include "Interface/DT_CombatInterface.h"

void UAN_RestoreAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IDT_StateInterface* StateInterface = Cast<IDT_StateInterface>(MeshComp->GetOwner());
		IDT_CombatInterface* CombatInterface = Cast<IDT_CombatInterface>(MeshComp->GetOwner());
		if (CombatInterface && StateInterface && StateInterface->GetLMBDown())
		{
			UE_LOG(LogTemp, Warning, TEXT("Call"));
			CombatInterface->DoAttack(StateInterface->GetRMBDown() ? "Ironsight" : "Hip");
		}
	}
}
