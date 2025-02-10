// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_NextComboAttack.h"
#include "Interface/DT_StateInterface.h"
#include "Interface/DT_CombatInterface.h"

void UANS_NextComboAttack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (MeshComp)
	{
		IDT_StateInterface* StateInterface = Cast<IDT_StateInterface>(MeshComp->GetOwner());
		if (StateInterface)
		{
			if (StateInterface->GetLMBDown())
			{
				StateInterface->SetActionState(EActionState::EAS_Unoccupied);
				StateInterface->RestoreState();

				IDT_CombatInterface* CombatInterface = Cast<IDT_CombatInterface>(MeshComp->GetOwner());
				if (CombatInterface)
					CombatInterface->DoAttack(SectionName);
			}
		}
	}
}
