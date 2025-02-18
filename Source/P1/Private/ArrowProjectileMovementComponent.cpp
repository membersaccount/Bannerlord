// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowProjectileMovementComponent.h"


UArrowProjectileMovementComponent::UArrowProjectileMovementComponent()
{
	InitialSpeed = 5000.0f;
	MaxSpeed = 5000.0f;
	ProjectileGravityScale = 0;
	//bRotationFollowsVelocity = true;
	bShouldBounce = false;
	//arrowProjectileMovementComponent->Bounciness = 0.3f;
	bSimulationEnabled = false;
	ProjectileGravityScale = 0.1f;


}

void UArrowProjectileMovementComponent::shotHandler(const FVector& Direction)
{
	this->bSimulationEnabled = true;
	this->Velocity = Direction * InitialSpeed;
}