// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "DT_VM_TeamScore.generated.h"

/**
 * 
 */
UCLASS()
class DISTORTION_API UDT_VM_TeamScore : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	void Init();

	UFUNCTION(BlueprintPure, FieldNotify)
	uint8 GetRedTeamScore() const { return RedTeamScore; }

	void SetRedTeamScore(const uint8 InRedTeamScore) {
		if (UE_MVVM_SET_PROPERTY_VALUE(RedTeamScore, InRedTeamScore))
			UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(RedTeamScore);
	}

	UFUNCTION(BlueprintPure, FieldNotify)
	uint8 GetBlueTeamScore() const { return BlueTeamScore; }

	void SetBlueTeamScore(const uint8 InBlueTeamScore) {
		if (UE_MVVM_SET_PROPERTY_VALUE(BlueTeamScore, InBlueTeamScore))
			UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(BlueTeamScore);
	}

protected:
	virtual void BindCallbacks();

protected:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	uint8 RedTeamScore;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	uint8 BlueTeamScore;
};
