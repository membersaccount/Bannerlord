// Fill out your copyright notice in the Description page of Project Settings.


#include "BowAnimInstance.h"
#include "../PlayerCharacter.h"
#include "WeaponComponent.h"
#include "ECharacterState.h"

void UBowAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	OnPlayMontageNotifyBegin.AddDynamic(this, &UBowAnimInstance::OnMontageNotifyBegin);
	player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter1"));
}

	void UBowAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
	{
		Super::NativeUpdateAnimation(DeltaSeconds);

		if (!player)return;
		if (player->eChractoerState == ECharacterState::IDLE) {
			Montage_Resume(CurrentMontage);
		}
		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter2"));

	}

void UBowAnimInstance::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	CurrentMontage = this->GetCurrentActiveMontage();
	if (NotifyName == FName(TEXT("None")))
	{
		// 몽타주 일시 정지
		if (CurrentMontage)
		{
			if(player->eChractoerState == ECharacterState::ATTACKING)
				Montage_Pause(CurrentMontage);
			UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter3"));

		}
	}



}