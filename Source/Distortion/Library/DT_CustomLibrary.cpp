// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_CustomLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"

float UDT_CustomLibrary::CalculateTheta(const FVector& A, const FVector& B)
{
	const double CosTheta = FVector::DotProduct(A, B);
	double Theta = UKismetMathLibrary::DegAcos(CosTheta);

	FVector Product = FVector::CrossProduct(A, B);
	float Sign = UKismetMathLibrary::SignOfFloat(Product.Z);

	Theta *= Sign;
	return Theta;
}

FName UDT_CustomLibrary::CheckSectionName_8Direction(const float& Theta)
{
	static const FData Sections[] = {
			  { {-22.5, 22.5}, FName("Fwd")},
			  { {22.5, 67.5}, FName("FR")},
			  { {67.5, 112.5}, FName("RT")},
			  { {112.5, 157.5}, FName("BR")},
			  { {-67.5, -22.5}, FName("FL")},
			  { {-112.5, -67.5}, FName("LT")},
			  { {-157.5, -112.5}, FName("BL")}
	};

	for (const auto& Section : Sections)
		if (Section.Range.Key <= Theta && Theta < Section.Range.Value)
			return Section.Name;

	return FName("Bwd");
}

FName UDT_CustomLibrary::CheckSectionName_4Direction(const float& Theta)
{
	static const FData Sections[] = {
			  {{-45, 45}, FName("Fwd")},
			  {{45, 135}, FName("RT")},
			  {{-135, -45}, FName("LT")},
	};

	for (const auto& Section : Sections)
		if (Section.Range.Key <= Theta && Theta < Section.Range.Value)
			return Section.Name;

	return FName("Bwd");
}
