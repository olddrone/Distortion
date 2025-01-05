// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "DT_VM_Attribute.generated.h"

class UDT_AttributeComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStatInitDelegate, const UDT_AttributeComponent*, Attributes);
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class DISTORTION_API UDT_VM_Attribute : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	void Init(APlayerController* InPlayerController, class ADT_PlayerState* InPlayerState, UDT_AttributeComponent* InAttributes);
   
    float GetHealth() const { return Health; }
    void SetHealth(float InHealth) { UE_MVVM_SET_PROPERTY_VALUE(Health, InHealth); }

    float GetMaxHealth() const { return MaxHealth; }
    void SetMaxHealth(float InMaxHealth) { UE_MVVM_SET_PROPERTY_VALUE(MaxHealth, InMaxHealth); }

protected:
    void NotifyInit();
    UFUNCTION(BlueprintImplementableEvent)
    void ReceiveInit();

    virtual void BindCallbacks();

    UFUNCTION()
    void OnHealthChanged(float InHealth);

protected:
    UPROPERTY()
    TObjectPtr<UDT_AttributeComponent> Attributes;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, FieldNotify, Setter, Getter)
    float Health;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, FieldNotify, Setter, Getter)
    float MaxHealth;
};
