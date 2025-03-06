// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_CollisionManager.h"
#include "Kismet/GameplayStatics.h"
#include "Library/DT_CustomLibrary.h"
#include "Interface/DT_CombatInterface.h"

UDT_CollisionManager::UDT_CollisionManager()
{
}

void UDT_CollisionManager::DoCollision(AActor* Caller)
{
	MeshInterface = TScriptInterface<IDT_MeshInterface>(Caller);
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

	TPair<FVector, FVector> CtrlPoint = CalculateControlPoints(BeforePoints.Key, CurStart, BeforePoints.Value, CurEnd);
	PerformInterpolatedTraces(BeforePoints.Key, CurStart, BeforePoints.Value, CurEnd, CtrlPoint, HitResults);


	HandleHitResults(HitResults);
	BeforePoints = { CurStart, CurEnd };
}


void UDT_CollisionManager::DoDamage(const FHitResult& Victim)
{
	if (UDT_CustomLibrary::SameTeamCheck(Pawn, Cast<APawn>(Victim.GetActor())))
		return;

	// FVector = 72Bit, FVector_NetQuantize = 60Bit
	FVector_NetQuantize InstigatorLocation = Pawn->GetActorLocation();

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
		EDrawDebugTrace::None, TempHitResults, true, Color, FColor::Green, 0.5f);
	HitResults.Append(TempHitResults);
}

void UDT_CollisionManager::DoSphereTrace(const FVector& Location, TArray<FHitResult>& HitResults, const FColor& Color)
{
	TArray<FHitResult> TempHitResults;
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), Location,Location, 15.f,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), false, ActorsToIgnore,
		EDrawDebugTrace::None, TempHitResults, true, Color, FColor::Green, 0.5f);
	HitResults.Append(TempHitResults);
}

TPair<FVector, FVector> UDT_CollisionManager::CalculateControlPoints(const FVector& PreStart, const FVector& CurStart,
	const FVector& PreEnd, const FVector& CurEnd)
{
	float DisStart = FVector::Dist(PreStart, CurStart);
	float DisEnd = FVector::Dist(PreEnd, CurEnd);

	FVector StartDir = (CurStart - PreStart).GetSafeNormal();
	FVector EndDir = (CurEnd - PreEnd).GetSafeNormal();

	float Ratio = 180.f / FMath::Max(FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(EndDir, StartDir))), 1.0f);

	uint8 StepBegin = FMath::CeilToInt(DisStart / Ratio);
	uint8 StepEnd = FMath::CeilToInt(DisEnd / Ratio);

	FVector CtrlPointStart = (PreStart + CurStart) * 0.5f;
	FVector CtrlPointEnd = (PreEnd + CurEnd) * 0.5f;
	FVector CtrlDir = (CtrlPointStart - CtrlPointEnd).GetSafeNormal();

	return TPair<FVector, FVector>{CtrlPointStart + CtrlDir * -StepBegin, CtrlPointEnd + CtrlDir * -StepEnd};
}

void UDT_CollisionManager::PerformInterpolatedTraces(const FVector& PreStart, const FVector& CurStart,
	const FVector& PreEnd, const FVector& CurEnd, TPair<FVector, FVector>& CtrlPoint,
	TArray<FHitResult>& HitResults)
{
	float DistanceStart = FVector::Dist(PreStart, CurStart);
	uint8 Steps = FMath::Max(FMath::CeilToInt(DistanceStart / 5.0f), 1);
	for (uint8 i = 1; i < Steps; ++i)
	{
		float T = static_cast<float>(i)/Steps;

		FVector StartLocation = UDT_CustomLibrary::BezierCurve_Quadratic(PreStart, CtrlPoint.Key, CurStart, T);
		FVector EndLocation = UDT_CustomLibrary::BezierCurve_Quadratic(PreEnd, CtrlPoint.Value, CurEnd, T);

		DoLineTrace(StartLocation, EndLocation, HitResults, FColor::Yellow);
	}
}
