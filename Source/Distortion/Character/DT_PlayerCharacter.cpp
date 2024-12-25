// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Controller/DT_PlayerController.h"
#include "DrawDebugHelpers.h"

#include "Components/CapsuleComponent.h"

ADT_PlayerCharacter::ADT_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.0f, 0.f));
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 300.f;
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
}

void ADT_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ADT_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	FVector Start = GetActorLocation();
	Start.Z = 5.f;
	const FVector Forward = GetActorForwardVector().GetSafeNormal();
	FVector End = (GetCharacterMovement()->Velocity).GetSafeNormal();
	FVector Tmp = GetLastMovementInputVector().GetSafeNormal();

	DrawDebugDirectionalArrow(GetWorld(), Start, Start + Forward * 100.f, 5.f, FColor::Red, false, -1.f, 0, 2.f);
	DrawDebugDirectionalArrow(GetWorld(), Start, Start + End * 100.f, 5.f, FColor::Green, false, -1.f, 0, 2.f);
	DrawDebugDirectionalArrow(GetWorld(), Start, Start + Tmp * 100.f, 5.f, FColor::Blue, false, -1.f, 0, 2.f);

	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = GetActorRotation().Pitch;
	ControlRotation.Roll = GetActorRotation().Roll;

	FVector ViewDirection = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X).GetSafeNormal();
	DrawDebugDirectionalArrow(GetWorld(), Start, Start + ViewDirection * 100.f, 5.f, FColor::Orange, false, -1.f, 0, 2.f);
}

void ADT_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	auto* controller = Cast<ADT_PlayerController>(GetController());

	if (IsValid(controller))
	{
		controller->DodgeDelegate.AddDynamic(this, &ADT_BaseCharacter::Dodge);
		controller->RMBDelegate.AddDynamic(this, &ADT_BaseCharacter::RMB);
		controller->LMBDelegate.AddDynamic(this, &ADT_BaseCharacter::Attack);
		controller->EquipDelegate.AddDynamic(this, &ADT_BaseCharacter::Equip);
	}
}
