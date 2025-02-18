// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EWeaponState.h"
#include "WeaponComponent.h"
#include "ECharacterState.h"
#include "ArrowActor.h"
#include "PlayerAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class P1_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeInitializeAnimation() override;

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	bool isInAir;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Speed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MoveRight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MoveForward;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EWeaponState weaponState;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool IsMove;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	ECharacterState characterState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AArrowActor* arrow;


	APlayerCharacter* player;
	UWeaponComponent* weaponComp;
	UAnimMontage* CurrentMontage;

UFUNCTION()
void OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

UFUNCTION()
void OnMontageNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);


void setCharacterMovement();

};
