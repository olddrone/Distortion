// Fill out your copyright notice in the Description page of Project Settings.


#include "DT_CollisionManager.h"
#include "Kismet/GameplayStatics.h"

UDT_CollisionManager::UDT_CollisionManager()
{
	PresentPoints.Init(FVector_NetQuantize::ZeroVector, 4);
	BeforePoints.Init(FVector_NetQuantize::ZeroVector, 4);
	
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

	for (auto& BeforePoint : BeforePoints)
		BeforePoint = FVector_NetQuantize::ZeroVector;
}

void UDT_CollisionManager::TraceCheck()
{
	PresentPoints[0] = MeshInterface->GetSocketLocation(StartSocketName);
	PresentPoints[3] = MeshInterface->GetSocketLocation(EndSocketName);
	PresentPoints[1] = FMath::Lerp(PresentPoints[0], PresentPoints[3], 1.0f / 3.0f);
	PresentPoints[2] = FMath::Lerp(PresentPoints[0], PresentPoints[3], 2.0f / 3.0f);

	TArray<FHitResult> HitResults;
	if (BeforePoints[0] != FVector_NetQuantize::ZeroVector)
	{
		for (int8 i = 0; i < PresentPoints.Num(); ++i)
			DoSphereTrace(BeforePoints[i], PresentPoints[i], HitResults, FColor::Orange);
	}

	for (const auto& HitResult : HitResults)
	{
		if (HitActors.Find(HitResult.GetActor()) == INDEX_NONE)
		{
			HitActors.Emplace(HitResult.GetActor());
			ServerRPCDoDamage(HitResult);
		}
	}

	for (int8 i = 0; i < BeforePoints.Num(); ++i)
		BeforePoints[i] = PresentPoints[i];
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
