// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DT_Crosshairs.generated.h"

class UTexture2D;

USTRUCT(BlueprintType)
struct FCrosshairsTextures
{
	GENERATED_BODY()

	FCrosshairsTextures& operator=(const FCrosshairsTextures& InTextures) {
		Center = InTextures.Center;
		Left = InTextures.Left;
		Right = InTextures.Right;
		Top = InTextures.Top;
		Bottom = InTextures.Bottom;
		Spread = InTextures.Spread;
		return *this;
	}

	FCrosshairsTextures& operator=(std::nullptr_t) {
		Center = Left = Right = Top = Bottom = nullptr; Spread = 0.f;
		return *this;
	}

	bool operator!=(std::nullptr_t) const {
		return (Center != nullptr && Left != nullptr && Right != nullptr && Top != nullptr && Bottom != nullptr);
	}

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> Center;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> Left;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> Right;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> Top;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> Bottom;

	UPROPERTY()
	float Spread;
};