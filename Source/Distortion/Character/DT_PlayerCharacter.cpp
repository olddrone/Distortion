// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Controller/DT_PlayerController.h"

#include "Camera/CameraComponent.h"
#include "PlayerState/DT_PlayerState.h"
#include "UI/HUD/DT_HUD.h"
#include "Component/DT_AttributeComponent.h"

#include "GameFramework/GameMode.h"
#include "TimerManager.h"
#include "Interface/DT_RespawnInterface.h"
#include "Interface/DT_CameraControlInterface.h"

ADT_PlayerCharacter::ADT_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 300.f;
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
}

void ADT_PlayerCharacter::RestoreState()
{
	Super::RestoreState(); 

	if (IsLocallyControlled() && GetEquipWeaponType() == EWeaponType::EWT_Gun)
		CameraInterface->SetZoom(bRMBDown);
}

void ADT_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAttributeComp(); // 리슨 서버, 서버에서 호출됨
}

void ADT_PlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAttributeComp(); // 클라, 서버에서 생성된 걸 복제
}

void ADT_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	BindingInterface();
}

void ADT_PlayerCharacter::InitAttributeComp()
{
	ADT_PlayerState* State = GetPlayerState<ADT_PlayerState>();
	if (State)
	{
		AttributeComp = State->GetAttributes();
		AttributeComp->InitValue();
		AttributeComp->Dead.AddUObject(this, &ADT_PlayerCharacter::Dead);

		const APlayerController* PC = GetController<APlayerController>();
		ADT_HUD* Hud = (PC) ? PC->GetHUD<ADT_HUD>() : nullptr;
		if (IsValid(Hud))
			Hud->InitOverlay(AttributeComp, CombatComp);

		SetTeamColor(State->GetTeam());
	}
}

void ADT_PlayerCharacter::BindingInterface()
{
	APlayerController* PC = GetController<APlayerController>();
	if (PC)
		CameraInterface = TScriptInterface<IDT_CameraControlInterface>(PC->PlayerCameraManager);
}

void ADT_PlayerCharacter::Hit(const FName& SectionName, const EAttackType& AttackType)
{
	Super::Hit(SectionName, AttackType);

	CameraInterface->DoHitCameraShake();
}

void ADT_PlayerCharacter::Dead()
{
	Super::Dead();
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
		PlayerController->DisableInput(nullptr); 

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &ADT_PlayerCharacter::Respawn, 3.0f, false);
}

void ADT_PlayerCharacter::RMB(bool bHoldRotationYaw)
{
	Super::RMB(bHoldRotationYaw);
	if(GetEquipWeaponType() == EWeaponType::EWT_Gun)
		CameraInterface->SetZoom(bRMBDown);
}

void ADT_PlayerCharacter::Respawn()
{
	IDT_RespawnInterface* Interface = Cast<IDT_RespawnInterface>(GetWorld()->GetAuthGameMode());
	if (Interface)
	{
		Interface->RequestPlayerRespawn(this, Controller);
		BindingInterface();
	}
}
