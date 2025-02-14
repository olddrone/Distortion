// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Data/DT_StateSet.h"
#include "DT_PlayerState.generated.h"

class UDT_AttributeComponent;

/**
 * 
 */
UCLASS()
class DISTORTION_API ADT_PlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ADT_PlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	FORCEINLINE UDT_AttributeComponent* GetAttributes() const { return Attributes; }
	FORCEINLINE ETeam GetTeam() const { return Team; }
	void SetTeam(const ETeam& TeamToSet);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDT_AttributeComponent> Attributes;

 	UPROPERTY(ReplicatedUsing = OnRep_Team, VisibleAnywhere, BlueprintReadOnly)
	ETeam Team = ETeam::ET_NoTeam;

	UFUNCTION()
	void OnRep_Team();
};
