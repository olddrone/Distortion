// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "DT_VM_Attribute.generated.h"

class UDT_AttributeComponent;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class DISTORTION_API UDT_VM_Attribute : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	void Init(UDT_AttributeComponent* InAttributes);
   
    UFUNCTION(BlueprintPure, FieldNotify)
    float GetHealthPercent() const { return (MaxHealth != 0) ? Health / MaxHealth : 0; }

    float GetHealth() const { return Health; }
    void SetHealth(float InHealth) {
        if (UE_MVVM_SET_PROPERTY_VALUE(Health, InHealth))
            UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
    }

    float GetMaxHealth() const { return MaxHealth; }
    void SetMaxHealth(float InMaxHealth) {
        if (UE_MVVM_SET_PROPERTY_VALUE(MaxHealth, InMaxHealth))
            UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
    }

    UFUNCTION(BlueprintPure, FieldNotify)
    float GetStaminaPercent() const { return (MaxStamina != 0) ? Stamina / MaxStamina : 0; }

    float GetStamina() const { return Stamina; }
    void SetStamina(float InStamina) {
        if (UE_MVVM_SET_PROPERTY_VALUE(Stamina, InStamina))
            UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStaminaPercent);
    }

    float GetMaxStamina() const { return MaxStamina; }
    void SetMaxStamina(float InMaxStamina) {
        if (UE_MVVM_SET_PROPERTY_VALUE(MaxStamina, InMaxStamina))
            UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStaminaPercent);
    }

protected:
    virtual void BindCallbacks();

protected:
    UPROPERTY()
    TObjectPtr<UDT_AttributeComponent> Attributes;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, FieldNotify, Setter, Getter)
    float Health;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, FieldNotify, Setter, Getter)
    float MaxHealth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, FieldNotify, Setter, Getter)
    float Stamina;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, FieldNotify, Setter, Getter)
    float MaxStamina;
};
