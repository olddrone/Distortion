// Fill out your copyright notice in the Description page of Project Settings.


#include "DT_ItemBox.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

ADT_ItemBox::ADT_ItemBox()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);

	OverlapComp = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapComp"));
	OverlapComp->SetupAttachment(GetRootComponent());

	NetUpdateFrequency = 1.0f;

}

void ADT_ItemBox::BeginPlay()
{
	Super::BeginPlay();

	OverlapComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &ADT_ItemBox::OnSphereOverlap);
}

void ADT_ItemBox::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!WeaponData)
		return;

	OverlapActor = Cast<IDT_InteractionInterface>(OtherActor);
	if (OverlapActor)
		OverlapActor->Interaction(WeaponData);
}