// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerAnimInstance.h"
#include "../PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WeaponComponent.h"
#include "ECharacterState.h"
#include "ArrowActor.h"
#include "WeaponActor.h"
#include "CollisionQueryParams.h"


void UPlayerAnimInstance::NativeInitializeAnimation()
{

}

void UPlayerAnimInstance::NativeBeginPlay()
{
	OnPlayMontageNotifyBegin.AddDynamic(this,&UPlayerAnimInstance::OnMontageNotifyBegin);
	OnPlayMontageNotifyEnd.AddDynamic(this,&UPlayerAnimInstance::OnMontageNotifyEnd);
	player = Cast<APlayerCharacter>(TryGetPawnOwner());
	weaponComp = player->WeaponComponent1;
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{

	Super::NativeUpdateAnimation(DeltaSeconds);
	setCharacterMovement();
	CurrentMontage = this->GetCurrentActiveMontage();

	if (!CurrentMontage) return;

	switch (characterState)
	{
	case ECharacterState::IDLE:
		Montage_Resume(CurrentMontage);
		break;
	case ECharacterState::DIE:

		break;
	}

}

void UPlayerAnimInstance::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == FName(TEXT("endAttack"))) {
		player->isAttack = false;
	}
	if (NotifyName == FName(TEXT("None")))
	{
		// 몽타주 일시 정지
		CurrentMontage = this->GetCurrentActiveMontage();
			if (CurrentMontage)
			{
				if (characterState == ECharacterState::ATTACKING) {
					Montage_Pause(CurrentMontage);
				}
			}
	}
	if (NotifyName == FName(TEXT("ArrowSpawn"))) {
		if (CurrentMontage)
		{
			player->spawnArrow();
		}
	}
	if (NotifyName == FName(TEXT("RemoveArrow"))) {

	}
	if (NotifyName == FName(TEXT("changeWeapon"))) {
		if (CurrentMontage)
		{
			player->WeaponComponent1->CurrentWeapon->selectWeapon();
		}
	}
	if (NotifyName == FName(TEXT("swordRemove"))) {
		if (CurrentMontage)
		{
			player->WeaponComponent1->CurrentWeapon->SwordMesh->SetVisibility(false);
			player->WeaponComponent1->CurrentWeapon->SpearMesh->SetVisibility(false);
		}
	}
	if (NotifyName == FName(TEXT("OnCollision"))) {
		if (!player->isAttack) return;
		if(weaponState==EWeaponState::SPEAR)
			player->CurWeapon->SpearMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		else if (weaponState == EWeaponState::SWORD)
		{
			player->CurWeapon->SwordMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}
	if (NotifyName == FName(TEXT("OffCollision"))) {
		if (weaponState == EWeaponState::SPEAR)
			player->CurWeapon->SpearMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		else if (weaponState == EWeaponState::SWORD)
		{
			player->CurWeapon->SwordMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	if (NotifyName == FName(TEXT("DieNotify"))) {
		CurrentMontage = this->GetCurrentActiveMontage();
		Montage_JumpToSection(FName("die"), CurrentMontage);
		player->eChractoerState = ECharacterState::DIE;
		player->bDead = true;
		UE_LOG(LogTemp, Log, TEXT("%f"),player->getHP());
	}
}

void UPlayerAnimInstance::OnMontageNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	player->isAttack = false;

	if (NotifyName == FName(TEXT("endAttack"))) {
		player->isAttack = false;
	}
}

void UPlayerAnimInstance::setCharacterMovement()
{
	if (!player || !weaponComp) return;
	MoveForward = FVector::DotProduct(player->GetVelocity(), player->GetActorForwardVector());
	MoveRight = FVector::DotProduct(player->GetVelocity(), player->GetActorRightVector());
	pitch = player->GetBaseAimRotation().Pitch;
	isInAir = player->GetCharacterMovement()->IsFalling();
	weaponState = weaponComp->weaponState;
	IsMove = player->GetCharacterMovement()->IsMovingOnGround();
	characterState = player->eChractoerState;

}

