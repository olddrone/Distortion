// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_Sword.h"
#include "Component/DT_CollisionManager.h"
#include "Component/DT_CombatComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetMathLibrary.h"

ADT_Sword::ADT_Sword()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(MeshComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	TrailEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailEffect"));
	TrailEffect->SetupAttachment(GetRootComponent());
}

void ADT_Sword::BeginPlay()
{
	Super::BeginPlay();

	const FVector BotPos = GetSocketLocation("StartSocket");
	const FVector TopPos = GetSocketLocation("EndSocket");

	TrailEffect->SetFloatParameter("RibbonWidth", (TopPos - BotPos).Length());
	TrailEffect->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(BotPos - TopPos));
	TrailEffect->SetWorldLocation((TopPos + BotPos) / 2);
	TrailEffect->Deactivate();
}

void ADT_Sword::Attack(const FDamagePacket& DamagePacket, const FVector_NetQuantize& TraceHitTarget)
{
	if (IsValid(CollisionManager))
	{
		CollisionManager->SetSocketName(DamagePacket.StartSocketName, DamagePacket.EndSocketName);
		CollisionManager->DoCollision(this);
	}
}

void ADT_Sword::AttackEnd()
{
	if (IsValid(CollisionManager))
		CollisionManager->StopCollision();

}

void ADT_Sword::Equip(APawn* OwnerPawn, const FName& InSocketName, UDT_CollisionManager* InCollisionManager)
{
	Super::Equip(OwnerPawn, InSocketName, InCollisionManager);

	CollisionManager = InCollisionManager;
	if (Cast<APawn>(OwnerPawn)->IsLocallyControlled())
	{
		CollisionManager->SetCharacter(OwnerPawn);
		CollisionManager->SetActorsToIgnore(OwnerPawn);
		CollisionManager->SetActorsToIgnore(this);
	}
}

void ADT_Sword::SetFXVisibility(const bool bVisible)
{
	if(bVisible)
		TrailEffect->Activate();
	else
		TrailEffect->Deactivate();
}

