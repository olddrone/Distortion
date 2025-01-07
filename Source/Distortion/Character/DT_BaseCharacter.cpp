// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Component/DT_AttributeComponent.h"
#include "Component/DT_CombatComponent.h"
#include "Library/DT_CustomLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

ADT_BaseCharacter::ADT_BaseCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
	
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.0f, 0.f));
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	// 이 둘은 항상 반대가 되어야 떨림이 없음
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed::Run;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;

	CombatComp = CreateDefaultSubobject<UDT_CombatComponent>(TEXT("CombatComponent"));
	
}

void ADT_BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() > ENetRole::ROLE_SimulatedProxy && IsLocallyControlled() && GetEquipWeaponType() == EWeaponType::EWT_Gun)
		AimOffset(DeltaTime);
	else
	{
		TimeSinceLastMovementReplication += DeltaTime;
		if (TimeSinceLastMovementReplication > 0.25f)
			OnRep_ReplicatedMovement();
	}
}

void ADT_BaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADT_BaseCharacter, bRMBDown);
}

void ADT_BaseCharacter::OnRep_ReplicatedMovement()
{
	Super::OnRep_ReplicatedMovement();
	if (CombatComp->GetEquipWeapon())
		SimProxiesTurn();
	TimeSinceLastMovementReplication = 0.f;
}

void ADT_BaseCharacter::AimOffset(const float& DeltaTime)
{
	const bool bIsInAir = GetCharacterMovement()->IsFalling();

	if (UKismetMathLibrary::VSizeXY(GetVelocity()) == 0.f && !bIsInAir)
	{
		bRotateRootBone = true;
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		AimOffsetYaw = DeltaAimRotation.Yaw;

		if (TurnInPlace == ETurnInPlace::ETIP_NotTurn)
			InterpAOYaw = AimOffsetYaw;

		SetRotationYaw(true);
		CheckTurnInPlace(DeltaTime);
	}
	else
	{
		bRotateRootBone = false;
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AimOffsetYaw = 0.f;
		SetRotationYaw(true);
		TurnInPlace = ETurnInPlace::ETIP_NotTurn;
	}
}

void ADT_BaseCharacter::CheckTurnInPlace(const float& DeltaTime)
{
	if (AimOffsetYaw > 90.f)
		TurnInPlace = ETurnInPlace::ETIP_Right;
	else if (AimOffsetYaw < -90.f)
		TurnInPlace = ETurnInPlace::ETIP_Left;

	if (TurnInPlace != ETurnInPlace::ETIP_NotTurn)
	{
		InterpAOYaw = FMath::FInterpTo(InterpAOYaw, 0.f, DeltaTime, 4.f);
		AimOffsetYaw = InterpAOYaw;
		if (FMath::Abs(AimOffsetYaw) < 15.f)
		{
			TurnInPlace = ETurnInPlace::ETIP_NotTurn;
			StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
}

void ADT_BaseCharacter::PlayMontage(UAnimMontage* Montage, const FName& SectionName)
{
	if (GetEquipWeaponType() != EWeaponType::EWT_Gun)
		SetRotationYaw(false);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

void ADT_BaseCharacter::LMB(bool bIsAttack)
{
	bLMBDown = bIsAttack;

	if (bLMBDown)
		Attack();
}

void ADT_BaseCharacter::RMB(bool bHoldRotationYaw)
{
	bRMBDown = bHoldRotationYaw;
	ServerRPCRMBDown(bHoldRotationYaw);

	if (GetActionState() != EActionState::EAS_Unocuupied)
		return;

	const float Value = (bRMBDown) ? 0.58f : 0.f;
	CombatComp->SetAimFactor(Value);
	GetCharacterMovement()->MaxWalkSpeed = (bRMBDown) ? WalkSpeed::Walk : WalkSpeed::Run;
	if (GetEquipWeaponType() != EWeaponType::EWT_Gun)
		SetRotationYaw(bHoldRotationYaw);
}

void ADT_BaseCharacter::ServerRPCRMBDown_Implementation(bool bHoldRotationYaw)
{
	bRMBDown = bHoldRotationYaw;
}

void ADT_BaseCharacter::SetRotationYaw(bool bHoldRotationYaw)
{
	if (bUseControllerRotationYaw == bHoldRotationYaw)
		return;
	if (IsLocallyControlled())
		ServerRPCSetRotationYaw(bHoldRotationYaw);
}

void ADT_BaseCharacter::ServerRPCSetRotationYaw_Implementation(bool bHoldRotationYaw)
{
	MulticastRPCSetRotationYaw(bHoldRotationYaw);
}

void ADT_BaseCharacter::MulticastRPCSetRotationYaw_Implementation(bool bHoldRotationYaw)
{
	bUseControllerRotationYaw = bHoldRotationYaw;
	GetCharacterMovement()->bOrientRotationToMovement = !bHoldRotationYaw;
}

FTransform ADT_BaseCharacter::GetWeaponSocketTransform(const FName& SocketName) const
{
	return CombatComp->GetMeshSocketTransform(SocketName);
}

void ADT_BaseCharacter::Dodge()
{
	if (GetActionState() != EActionState::EAS_Unocuupied)
		return;

	SetActionState(EActionState::EAS_Dodge);
	if (CombatComp)
	{
		if (bRMBDown || GetEquipWeaponType() == EWeaponType::EWT_Gun)
		{
			FVector ForwardVector = GetActorForwardVector();
			FVector InputVector = GetLastMovementInputVector().GetSafeNormal();
			float Theta = UDT_CustomLibrary::CalculateTheta(ForwardVector, { InputVector.X,InputVector.Y,0.f });
			CombatComp->Dodge(UDT_CustomLibrary::CheckSectionName_8Direction(Theta));
		}
		else
		{
			ImmediateRotate();
			CombatComp->Dodge();
		}
	}
}

void ADT_BaseCharacter::Hit(const FName& SectionName)
{
	SetActionState(EActionState::EAS_Hit);
	CombatComp->Hit(SectionName);
}

void ADT_BaseCharacter::ImmediateRotate()
{
	if (bRMBDown) // 현재 보고 있는 방향으로 회전
	{
		FRotator Rotation = GetControlRotation();
		Rotation.Pitch = GetActorRotation().Pitch;
		Rotation.Roll = GetActorRotation().Roll;
		ServerRPCRotate(Rotation.Quaternion());

	}
	else // 키보드 입력 방향에 따라 회전
	{
		FVector InputVector = GetLastMovementInputVector();
		if (InputVector.Size() > KINDA_SMALL_NUMBER)
		{
			FRotator Rotation = InputVector.Rotation();
			ServerRPCRotate(Rotation.Quaternion());
		}
	}
}

void ADT_BaseCharacter::ServerRPCRotate_Implementation(const FQuat4d& Quat)
{
	MulticastRPCRotate(Quat);
}

void ADT_BaseCharacter::MulticastRPCRotate_Implementation(const FQuat4d& Quat)
{
	SetActorRotation(Quat);
}

void ADT_BaseCharacter::Equip()
{
	if (GetActionState() != EActionState::EAS_Unocuupied)
		return;
	if (!CombatComp->GetHasEquipWeapon())
		return;

	SetActionState(EActionState::EAS_Equip);
	CombatComp->EquipCheck();
}

void ADT_BaseCharacter::Attack()
{
	FName SectionName; 
	if (GetEquipWeaponType() != EWeaponType::EWT_Gun)
		SectionName = "Attack01";
	else
		SectionName = (bRMBDown)? "Ironsight": "Hip";

	DoAttack(SectionName);
}

void ADT_BaseCharacter::DoAttack(const FName& SectionName)
{
	if (GetActionState() != EActionState::EAS_Unocuupied)
		return;

	SetActionState(EActionState::EAS_Attack);

	if (GetEquipWeaponType() != EWeaponType::EWT_Gun)
		ImmediateRotate();
	CombatComp->Attack(SectionName);
}

void ADT_BaseCharacter::ActivateCollision(const FDamagePacket& DamagePacket)
{
	AnimTickOption(EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones);
	CombatComp->CollisionStart(DamagePacket);
}

void ADT_BaseCharacter::DeactivateCollision()
{
	AnimTickOption(EVisibilityBasedAnimTickOption::AlwaysTickPose);
	CombatComp->CollisionEnd();
}

void ADT_BaseCharacter::GetHit(const FVector_NetQuantize& InstigatorLocation, const int8& DamageAmount, const FDamagePacket& DamagePacket)
{
	if (HasAuthority())
	{
		AttributeComp->ApplyDamage(DamageAmount);
		ClientRPCGetHit(InstigatorLocation, DamageAmount, DamagePacket);
	}
}

// 해당 클라에서만 실행
void ADT_BaseCharacter::ClientRPCGetHit_Implementation(const FVector_NetQuantize& InstigatorLocation, 
	const int8& DamageAmount, const FDamagePacket& DamagePacket)
{
	if (IsLocallyControlled())
	{
		FVector Forward = GetActorForwardVector().GetSafeNormal();
		FVector ToInstigator = (InstigatorLocation - GetActorLocation()).GetSafeNormal();
		float Theta = UDT_CustomLibrary::CalculateTheta(Forward, ToInstigator);
		FName Section = UDT_CustomLibrary::CheckSectionName_4Direction(Theta);


		// 여기서 가드 체크
		if (bRMBDown && GetEquipWeaponType() == EWeaponType::EWT_Sword && Section != "Bwd")
		{
			FName Text = "Default";
			EAttackDirection Direction = DamagePacket.AttackDirection;
			if ((Section == "Fwd" && Direction == EAttackDirection::EAD_Right) ||
				(Section == "RT" && Direction != EAttackDirection::EAD_Left))
				Text = "RT";
			else if ((Section == "Fwd" && Direction == EAttackDirection::EAD_Left) ||
				(Section == "LT" && Direction != EAttackDirection::EAD_Right))
				Text = "LT";

			Guard(Text);
		}
		else
		{
			Hit(Section);
		}
	}
}

void ADT_BaseCharacter::Interaction(UDataAsset* DataAsset)
{
	if (!CombatComp->GetHasEquipWeapon())
		CombatComp->CreateWeapon(DataAsset);
}

void ADT_BaseCharacter::ToAttachSocket(const FName& SocketName)
{
	CombatComp->AttachSocket(SocketName);
}

void ADT_BaseCharacter::SimProxiesTurn()
{
	bRotateRootBone = false;
	float Speed = UKismetMathLibrary::VSizeXY(GetVelocity());
	if (Speed > 0.f)
	{
		TurnInPlace = ETurnInPlace::ETIP_NotTurn;
		return;
	}

	ProxyRotationLastFrame = ProxyRotation;
	ProxyRotation = GetActorRotation();
	ProxyYaw = UKismetMathLibrary::NormalizedDeltaRotator(ProxyRotation, ProxyRotationLastFrame).Yaw;

	if (FMath::Abs(ProxyYaw) > TurnThreshold)
	{
		if (ProxyYaw > TurnThreshold)
			TurnInPlace = ETurnInPlace::ETIP_Right;
		else if (ProxyYaw < -TurnThreshold)
			TurnInPlace = ETurnInPlace::ETIP_Left;
		else
			TurnInPlace = ETurnInPlace::ETIP_NotTurn;

		return;
	}
	TurnInPlace = ETurnInPlace::ETIP_NotTurn;
}

void ADT_BaseCharacter::AnimTickOption(const EVisibilityBasedAnimTickOption& AnimTickOption)
{
	if (HasAuthority())
		MulticastRPCAnimTickOption(AnimTickOption);
	else if (IsLocallyControlled())
		ServerRPCAnimTickOption(AnimTickOption);
}

void ADT_BaseCharacter::Guard(const FName& SectionName)
{
	SetActionState(EActionState::EAS_Guard);
	CombatComp->Guard(SectionName);
}

void ADT_BaseCharacter::ServerRPCAnimTickOption_Implementation(const EVisibilityBasedAnimTickOption& AnimTickOption)
{
	MulticastRPCAnimTickOption(AnimTickOption);
}

void ADT_BaseCharacter::MulticastRPCAnimTickOption_Implementation(const EVisibilityBasedAnimTickOption& AnimTickOption)
{
	GetMesh()->VisibilityBasedAnimTickOption = AnimTickOption;
}
