// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WeaponComponent.h"
#include "BowAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class P1_API UBowAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:

	UAnimMontage* CurrentMontage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class APlayerCharacter* player;


	UFUNCTION()
	void OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

};
