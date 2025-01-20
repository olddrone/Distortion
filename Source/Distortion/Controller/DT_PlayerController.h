// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Data/PDA_Input.h"
#include "Interface/DT_StateInterface.h"
#include "DT_PlayerController.generated.h"

struct FInputActionValue;

/**
 * 
 */
UCLASS()
class DISTORTION_API ADT_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ADT_PlayerController();
	
	virtual void DisableInput(class APlayerController* PlayerController) override;

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
	void SetZoom(const bool& bIsZoom);

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPDA_Input> InputData = nullptr;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ADT_PlayerCharacter> PlayerCharacter = nullptr;

	// TScriptInterface<ILT_StateInterface>
	IDT_StateInterface* StateInterface;

};
