// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DT_GameState.generated.h"

DECLARE_DELEGATE_OneParam(FTeamScoreChangeDelegate, uint8);
/**
 * 
 */
UCLASS()
class DISTORTION_API ADT_GameState : public AGameState
{
	GENERATED_BODY()
	friend class ADT_GameMode;
	friend class UDT_VM_TeamScore;
public:
	// ADT_GameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_RedTeamScore();

	UFUNCTION()
	void OnRep_BlueTeamScore();

	void RedTeamInc();
	void BlueTeamInc();

	FORCEINLINE uint8 GetRedTeamScore() const { return RedTeamScore; }
	FORCEINLINE uint8 GetBlueTeamScore() const { return BlueTeamScore; }

protected:
	virtual void BeginPlay() override;

	FTeamScoreChangeDelegate RedTeamScoreChange;
	FTeamScoreChangeDelegate BlueTeamScoreChange;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_RedTeamScore, VisibleAnywhere, BlueprintReadOnly)
	uint8 RedTeamScore = 0;

	UPROPERTY(ReplicatedUsing = OnRep_BlueTeamScore, VisibleAnywhere, BlueprintReadOnly)
	uint8 BlueTeamScore = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	uint8 WinScore = 3;

private:
	UPROPERTY(EditAnywhere, meta = (PrivateAllowAccess = "true"))
	TSubclassOf<AActor> PoolActor;

	UPROPERTY(EditAnywhere, meta = (PrivateAllowAccess = "true"))
	uint32 PoolSize;

	TArray<APlayerState*> RedTeam;
	TArray<APlayerState*> BlueTeam;
};
