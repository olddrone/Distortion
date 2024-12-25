// Fill out your copyright notice in the Description page of Project Settings.


#include "DT_CollisionManager.h"
#include "Kismet/GameplayStatics.h"

UDT_CollisionManager::UDT_CollisionManager()
{
	
}

void UDT_CollisionManager::BeginPlay()
{
	Super::BeginPlay();

}

void UDT_CollisionManager::DoCollision(AActor* Caller)
{
	MeshInterface = Cast<IDT_MeshInterface>(Caller);
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &UDT_CollisionManager::TraceCheck, InRate, true);
}

void UDT_CollisionManager::StopCollision()
{
	GetWorld()->GetTimerManager().ClearTimer(Handle);
	HitActors.Empty();
	BeforeStart = BeforeEnd = FVector::ZeroVector;
}

void UDT_CollisionManager::TraceCheck()
{
	static FVector PresentStart(FVector::ZeroVector), PresentEnd(FVector::ZeroVector);

	PresentStart = MeshInterface->GetSocketLocation(StartSocketName);
	PresentEnd = MeshInterface->GetSocketLocation(EndSocketName);

	TArray<FHitResult> HitResults;
	if (!BeforeStart.IsZero() && !BeforeEnd.IsZero())
	{
		DoSphereTrace(BeforeStart, PresentStart, HitResults, FColor::Orange);
		DoSphereTrace(BeforeEnd, PresentEnd, HitResults, FColor::Orange);
	}
	DoSphereTrace(PresentStart, PresentEnd, HitResults, FColor::Orange);

	for (const auto& HitResult : HitResults)
	{
		if (HitActors.Find(HitResult.GetActor()) == INDEX_NONE)
		{
			HitActors.Emplace(HitResult.GetActor());
			ServerRPCDoDamage(HitResult);
		}
	}
	BeforeStart = PresentStart;
	BeforeEnd = PresentEnd;
}

void UDT_CollisionManager::DoSphereTrace(const FVector& StartLocation, const FVector& EndLocation, TArray<FHitResult>& HitResults, const FColor& Color)
{
	TArray<FHitResult> TempHitResults;
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), StartLocation, EndLocation, 15.f,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), false, ActorsToIgnore,
		EDrawDebugTrace::ForDuration, TempHitResults, true, Color, FColor::Green, 0.5f);

	HitResults.Append(TempHitResults);
}

void UDT_CollisionManager::ServerRPCDoDamage_Implementation(const FHitResult& Victim)
{
	if (!Character)
		return;
	// FVector = 72Bit, FVector_NetQuantize = 60Bit
	FVector_NetQuantize InstigatorLocation = Character->GetActorLocation();

	IDT_CombatInterface* VictimInterface = Cast<IDT_CombatInterface>(Victim.GetActor());
	if (VictimInterface)
		VictimInterface->GetHit(InstigatorLocation, 10);
}
