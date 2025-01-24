// Fill out your copyright notice in the Description page of Project Settings.

#include "DT_BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Component/DT_AttributeComponent.h"
#include "Component/DT_CombatComponent.h"
#include "Library/DT_CustomLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
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

void ADT_BaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ADT_BaseCharacter, bRMBDown, COND_SimulatedOnly);
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
	CombatComp->SetAimFactor(bHoldRotationYaw ? 0.58f : 0.f);

	if (GetActionState() != EActionState::EAS_Unocuupied)
		return;

	SetRotationYaw(bRMBDown);
	ServerRPCRMBDown(bHoldRotationYaw);
}


void ADT_BaseCharacter::ServerRPCRMBDown_Implementation(bool bRMB)
{
	bRMBDown = bRMB;
	SetRotationYaw(bRMBDown); // 서버에선 OnRep이 호출 안되므로 별도로 호출
}

void ADT_BaseCharacter::OnRep_RMBDown()
{
	SetRotationYaw(bRMBDown);
}

void ADT_BaseCharacter::SetRotationYaw(bool bHoldRotationYaw)
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

void ADT_BaseCharacter::ImmediateRotate()
{
	// 오른쪽이면 cos, 왼쪽이면 acos로 바꿔야
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
	if (GetActionState() != EActionState::EAS_Unocuupied || !CombatComp->GetHasEquipWeapon())
		return;

	SetActionState(EActionState::EAS_Equip);
	CombatComp->EquipCheck();
}

void ADT_BaseCharacter::Reload()
{
	if (GetActionState() != EActionState::EAS_Unocuupied ||
		!CombatComp->GetHasEquipWeapon() || GetEquipWeaponType() != EWeaponType::EWT_Gun)
		return;

	SetActionState(EActionState::EAS_Reload);
	CombatComp->Reload();
}

void ADT_BaseCharacter::Attack()
{
	// 몽타주 섹션도 CombatComp에서 나누는 게
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
	if(GetNetMode() == ENetMode::NM_DedicatedServer)
		AnimTickOption(EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones);
	CombatComp->CollisionStart(DamagePacket);
}

void ADT_BaseCharacter::DeactivateCollision()
{
	if (GetNetMode() == ENetMode::NM_DedicatedServer)
		AnimTickOption(EVisibilityBasedAnimTickOption::AlwaysTickPose);
	CombatComp->CollisionEnd();
}

void ADT_BaseCharacter::GetHit(const FVector_NetQuantize& InstigatorLocation, const int8& DamageAmount, const FDamagePacket& DamagePacket)
{
	if (HasAuthority())
	{
		AttributeComp->ApplyDamage(DamageAmount);
		if (AttributeComp->GetHealth() > 0.f)
			ClientRPCGetHit(InstigatorLocation, DamageAmount, DamagePacket);
	}
}

void ADT_BaseCharacter::ClientRPCGetHit_Implementation(const FVector_NetQuantize& InstigatorLocation, 
	const int8& DamageAmount, const FDamagePacket& DamagePacket)
{ 
	if (IsLocallyControlled())
	{
		FVector Forward = GetActorForwardVector().GetSafeNormal();
		FVector ToInstigator = (InstigatorLocation - GetActorLocation()).GetSafeNormal();
		float Theta = UDT_CustomLibrary::CalculateTheta(Forward, ToInstigator);
		FName Section = UDT_CustomLibrary::CheckSectionName_4Direction(Theta);

		(bRMBDown && GetEquipWeaponType() == EWeaponType::EWT_Sword && Section != "Bwd")
			? Guard(UDT_CustomLibrary::CheckSectionName_Guard(Section, (uint8)DamagePacket.AttackDirection))
			: Hit(Section, DamagePacket.AttackType);
	}
}

void ADT_BaseCharacter::Hit(const FName& SectionName, const EAttackType& AttackType)
{
	if (GetActionState() == EActionState::EAS_Dead)
		return;
	SetActionState(EActionState::EAS_Hit);

	CombatComp->Hit(SectionName, AttackType);
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

void ADT_BaseCharacter::Guard(const FName& SectionName)
{
	SetActionState(EActionState::EAS_Guard);
	CombatComp->Guard(SectionName);
}

void ADT_BaseCharacter::AnimTickOption(const EVisibilityBasedAnimTickOption& AnimTickOption)
{
	ServerRPCAnimTickOption(AnimTickOption);
}

void ADT_BaseCharacter::ServerRPCAnimTickOption_Implementation(const EVisibilityBasedAnimTickOption& AnimTickOption)
{
	GetMesh()->VisibilityBasedAnimTickOption = AnimTickOption;
}

void ADT_BaseCharacter::Dead()
{
	SetActionState(EActionState::EAS_Dead);
	CombatComp->DestroyWeapon();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);

}
