
// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_CollisionManager.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/DT_CombatInterface.h"

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
	DamagePacket = nullptr;
	BeforePoints = { FVector::ZeroVector,FVector::ZeroVector };
}

void UDT_CollisionManager::TraceCheck()
{
    TArray<FHitResult> HitResults;
    FVector CurStart = MeshInterface->GetSocketLocation(DamagePacket.StartSocketName);
    FVector CurEnd = MeshInterface->GetSocketLocation(DamagePacket.EndSocketName);

	(CurStart == CurEnd) 
		? DoSphereTrace(CurStart, HitResults, FColor::Blue)
		: DoLineTrace(CurStart, CurEnd, HitResults, FColor::Blue);

	if (BeforePoints.Key.IsZero() || BeforePoints.Value.IsZero())
	{
		BeforePoints = { CurStart, CurEnd };
		return;
	}

	DoLineTrace(BeforePoints.Key, CurStart, HitResults, FColor::Blue);
	DoLineTrace(BeforePoints.Value, CurEnd, HitResults, FColor::Blue);
	
	TPair<FVector,FVector> CtrlPos;
	CalculateControlPoints(BeforePoints.Key, CurStart, BeforePoints.Value, CurEnd, CtrlPos);

	PerformInterpolatedTraces(BeforePoints.Key, CurStart, BeforePoints.Value, CurEnd,CtrlPos, HitResults);

	HandleHitResults(HitResults);
	BeforePoints = { CurStart, CurEnd };
}

void UDT_CollisionManager::DoDamage(const FHitResult& Victim)
{
	if (!Character)
		return;
	// FVector = 72Bit, FVector_NetQuantize = 60Bit
	FVector_NetQuantize InstigatorLocation = Character->GetActorLocation();

	IDT_CombatInterface* VictimInterface = Cast<IDT_CombatInterface>(Victim.GetActor());
	if (VictimInterface)
		VictimInterface->GetHit(InstigatorLocation, Damage, DamagePacket);
}

void UDT_CollisionManager::HandleHitResults(const TArray<FHitResult>& HitResults)
{    
	// 충돌 액터 처리
	for (const auto& HitResult : HitResults)
	{
		if (HitActors.Find(HitResult.GetActor()) == INDEX_NONE)
		{
			HitActors.Emplace(HitResult.GetActor());
			DoDamage(HitResult);
		}
	}
}

void UDT_CollisionManager::DoLineTrace(const FVector& StartLocation, const FVector& EndLocation, TArray<FHitResult>& HitResults, const FColor& Color)
{
	TArray<FHitResult> TempHitResults;
	UKismetSystemLibrary::LineTraceMulti(GetWorld(), StartLocation, EndLocation,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), false, ActorsToIgnore,
		EDrawDebugTrace::ForDuration, TempHitResults, true, Color, FColor::Green, 0.5f);
	HitResults.Append(TempHitResults);
}

void UDT_CollisionManager::DoSphereTrace(const FVector& Location, TArray<FHitResult>& HitResults, const FColor& Color)
{
	TArray<FHitResult> TempHitResults;
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), Location,Location, 15.f,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), false, ActorsToIgnore,
		EDrawDebugTrace::ForDuration, TempHitResults, true, Color, FColor::Green, 0.5f);
	HitResults.Append(TempHitResults);
}

FVector UDT_CollisionManager::BezierCurve(const FVector& P0, const FVector& P1, const FVector& P2, float Alpha) const
{
	float OneMinusT = 1.0f - Alpha;
	return (OneMinusT * OneMinusT * P0) + (2 * OneMinusT * Alpha * P1) + (Alpha * Alpha * P2);
}

void UDT_CollisionManager::CalculateControlPoints(const FVector& PreStart, const FVector& CurStart, 
	const FVector& PreEnd, const FVector& CurEnd, TPair<FVector, FVector>& CtrlPos)
{
	float DisStart = FVector::Dist(PreStart, CurStart);
	float DisEnd = FVector::Dist(PreEnd, CurEnd);

	FVector StartDir = (CurStart - PreStart).GetSafeNormal();
	FVector EndDir = (CurEnd - PreEnd).GetSafeNormal();

	float Dot = FVector::DotProduct(EndDir, StartDir);
	float Radian = FMath::Acos(Dot);
	float Degree = FMath::RadiansToDegrees(Radian);
	
	float Ratio = 180.f / (Degree);
	float InterpStep = 10.0f;

	int32 NumStepsStart = FMath::CeilToInt(DisStart / Ratio);
	int32 NumStepsEnd = FMath::CeilToInt(DisEnd / Ratio);
	int32 NumSteps = FMath::CeilToInt(DisStart / InterpStep);

	FVector CtrlPosStart = (PreStart + CurStart) / 2.0f;
	FVector CtrlPosEnd = (PreEnd + CurEnd) / 2.0f;
	FVector CtrlDir = (CtrlPosStart - CtrlPosEnd).GetSafeNormal();
	
	CtrlPos = { CtrlPosStart + CtrlDir * -NumStepsStart,CtrlPosEnd + CtrlDir * -NumStepsEnd };
}

void UDT_CollisionManager::PerformInterpolatedTraces(const FVector& PreStart, const FVector& CurStart,
	const FVector& PreEnd, const FVector& CurEnd, TPair<FVector, FVector>& CtrlPos,
	TArray<FHitResult>& HitResults)
{
	float DistanceStart = FVector::Dist(PreStart, CurStart);
	int32 NumSteps = FMath::Max(FMath::CeilToInt(DistanceStart / 10.0f), 1);
	for (int32 Step = 1; Step < NumSteps; ++Step)
	{
		float Alpha = static_cast<float>(Step) / static_cast<float>(NumSteps);

		FVector InterpStart = BezierCurve(PreStart, CtrlPos.Key, CurStart, Alpha);
		FVector InterpEnd = BezierCurve(PreEnd, CtrlPos.Value, CurEnd, Alpha);

		DoLineTrace(InterpStart, InterpEnd, HitResults, FColor::Yellow);
	}
}

