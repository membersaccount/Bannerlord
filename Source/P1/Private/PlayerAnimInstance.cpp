// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerAnimInstance.h"
#include "../PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WeaponComponent.h"
#include "ECharacterState.h"


void UPlayerAnimInstance::NativeInitializeAnimation()
{

	player = Cast<APlayerCharacter>(TryGetPawnOwner());
	weaponComp = player->WeaponComponent;
}

void UPlayerAnimInstance::NativeBeginPlay()
{
	OnPlayMontageNotifyBegin.AddDynamic(this,&UPlayerAnimInstance::OnMontageNotifyBegin);
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

