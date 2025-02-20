// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


UArrowProjectileMovementComponent::UArrowProjectileMovementComponent()
{
	InitialSpeed = 0.0f;
	MaxSpeed = 5000.0f;
	ProjectileGravityScale = 0;
	bRotationFollowsVelocity = true;
	bShouldBounce = false;
	//arrowProjectileMovementComponent->Bounciness = 0.3f;
	bSimulationEnabled = false;
	ProjectileGravityScale = 1.0f;


}

void UArrowProjectileMovementComponent::shotHandler(const FVector& TargetLocation)
{

	FVector StartLocation = this->UpdatedComponent->GetComponentLocation();
	FVector LaunchVelocity;

	bool bHaveLaunchVelocity = calculateLaunchVelocity(LaunchVelocity, StartLocation, TargetLocation);
	if (bHaveLaunchVelocity)
	{
		this->Velocity = LaunchVelocity;
		this->bSimulationEnabled = true;

		FRotator LaunchRotation = LaunchVelocity.Rotation();
		this->UpdatedComponent->SetWorldRotation(LaunchRotation);
	}
}

bool UArrowProjectileMovementComponent::calculateLaunchVelocity(FVector& OutLaunchVelocity, const FVector& StartLocation, const FVector& TargetLocation) const
{
	// A default launch speed
	float LaunchSpeed = 5000.0f; // Set a default launch speed or calculate based on distance

	// Use UGameplayStatics::SuggestProjectileVelocity to calculate the launch velocity
	return UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLaunchVelocity,
		StartLocation,
		TargetLocation,
		LaunchSpeed,
		false, // Use high arc
		0.0f, // Collision radius
		0.0f, // Override gravity Z
		ESuggestProjVelocityTraceOption::DoNotTrace
	);
}

