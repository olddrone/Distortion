// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/DT_BaseCharacter.h"
#include "Interface/DT_CameraControlInterface.h"
#include "DT_PlayerCharacter.generated.h"

class UDT_CrosshairComponent;
/**
 * 
 */
UCLASS()
class DISTORTION_API ADT_PlayerCharacter : public ADT_BaseCharacter
{
	GENERATED_BODY()

public:
	ADT_PlayerCharacter();
	virtual void RestoreState() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual void Hit(const FName& SectionName, const EAttackType& AttackType) override;

	virtual void Dead() override;
	virtual void RMB(bool bHoldRotationYaw) override;

	virtual void DoAttack(const FName& SectionName = "Attack01") override;
	virtual float GetSpread() const override;

protected:
	virtual void BeginPlay() override;
	void Respawn();
	void InitAttributeComp();
	void BindingInterface();

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> CameraComponent;

	UPROPERTY()
	TScriptInterface<IDT_CameraControlInterface> CameraInterface;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDT_CrosshairComponent> CrosshairComp;
};
