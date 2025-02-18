// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ArrowProjectileMovementComponent.generated.h"

/**
 *
 */
UCLASS()
class P1_API UArrowProjectileMovementComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()

public:
	UArrowProjectileMovementComponent();

    UFUNCTION(BlueprintCallable, Category = "Movement")
	void shotHandler(const FVector& TargetLocation);

private:
	bool calculateLaunchVelocity(FVector& OutLaunchVelocity, const FVector& StartLocation, const FVector& TargetLocation) const;
};


