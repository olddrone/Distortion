// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Data/PDA_Input.h"
#include "Interface/DT_StateInterface.h"
#include "DT_PlayerController.generated.h"

struct FInputActionValue;

DECLARE_DELEGATE(FEquipDelegate);
DECLARE_DELEGATE_OneParam(FLMBDelegate, bool);
DECLARE_DELEGATE_OneParam(FRMBDelegate, bool);
DECLARE_DELEGATE(FDodgeDelegate);

/**
 * 
 */
UCLASS()
class DISTORTION_API ADT_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ADT_PlayerController();

	FDodgeDelegate DodgeDelegate;
	FLMBDelegate LMBDelegate;
	FRMBDelegate RMBDelegate;
	FEquipDelegate EquipDelegate;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& Value);
	void Jump();
	void Dodge();
	void Crouch();

	void RMBStart();
	void RMBEnd();

	void LMBStart();
	void LMBEnd();

	void Equip();

private:
	UPROPERTY(EditAnywhere, Category = "Data", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPDA_Input> InputData = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ACharacter> ControlledCharacter = nullptr;

	// TScriptInterface<ILT_StateInterface>
	IDT_StateInterface* StateInterface;
};
