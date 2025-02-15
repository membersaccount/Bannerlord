// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerAnimInstance.h"
#include "../PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WeaponComponent.h"
#include "ECharacterState.h"
#include "ArrowActor.h"
#include "WeaponActor.h"


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
	if (characterState == ECharacterState::IDLE) {
		Montage_Resume(CurrentMontage);
	}

}

void UPlayerAnimInstance::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
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
			player->arrow->setVisibility(true);
		}
	}
	if (NotifyName == FName(TEXT("RemoveArrow"))) {
		if (CurrentMontage)
		{
			player->arrow->setVisibility(false);
		}
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
		player->CurWeapon->SpearMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	if (NotifyName == FName(TEXT("OffCollision"))) {
		player->CurWeapon->SpearMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}




}

void UPlayerAnimInstance::OnMontageNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{

}

void UPlayerAnimInstance::setCharacterMovement()
{
	if (!player || !weaponComp) return;
	MoveForward = FVector::DotProduct(player->GetVelocity(), player->GetActorForwardVector());
	MoveRight = FVector::DotProduct(player->GetVelocity(), player->GetActorRightVector());

	isInAir = player->GetCharacterMovement()->IsFalling();
	weaponState = weaponComp->weaponState;
	IsMove = player->GetCharacterMovement()->IsMovingOnGround();
	characterState = player->eChractoerState;


}

