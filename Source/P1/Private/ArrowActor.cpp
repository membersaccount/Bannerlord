// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowActor.h"
#include "Components/StaticMeshComponent.h"
#include "../PlayerCharacter.h"
#include "ArrowProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
AArrowActor::AArrowActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon"));
	SetRootComponent(SceneComp);

	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow"));
	ArrowMesh->SetupAttachment(SceneComp);

	ArrowMesh->SetRelativeScale3D(FVector(1.3));
	arrowProjectileMovementComponent = CreateDefaultSubobject<UArrowProjectileMovementComponent>(TEXT("arrowProjectileMovementComponent"));
	arrowProjectileMovementComponent->SetUpdatedComponent(RootComponent);

}

// Called when the game starts or when spawned
void AArrowActor::BeginPlay()
{
	Super::BeginPlay();
	PreviousLocation = arrowProjectileMovementComponent->UpdatedComponent->GetComponentLocation();
}

// Called every frame
void AArrowActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector CurrentLocation = arrowProjectileMovementComponent->UpdatedComponent->GetComponentLocation();

	// Draw a debug line between the previous location and the current location
	DrawDebugLine(
		GetWorld(),
		PreviousLocation,
		CurrentLocation,
		FColor::Green,
		false, // Persistent (never goes away)
		-1.f,  // Lifetime (negative value = infinite)
		0,     // Depth priority
		1.f    // Thickness
	);
}
