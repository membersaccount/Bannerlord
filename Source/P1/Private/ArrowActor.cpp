// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowActor.h"
#include "Components/StaticMeshComponent.h"
#include "../PlayerCharacter.h"
#include "ArrowProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Characters/MBAISpearman.h"
#include "TimerManager.h"
#include "WeaponComponent.h"
#include "Components/Image.h"

// Sets default values
AArrowActor::AArrowActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon"));
	SetRootComponent(SceneComp);

	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow"));
	ArrowMesh->SetupAttachment(SceneComp);

	ArrowMesh->SetRelativeScale3D(FVector(1.5));
	arrowProjectileMovementComponent = CreateDefaultSubobject<UArrowProjectileMovementComponent>(TEXT("arrowProjectileMovementComponent"));
	arrowProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	ArrowMesh->SetCollisionProfileName(TEXT("arrow"));
	ArrowMesh->OnComponentBeginOverlap.AddDynamic(this, &AArrowActor::overlapEvent);

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

void AArrowActor::overlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMBAISpearman* Enemy = Cast<AMBAISpearman>(OtherActor);
	if (Enemy) {
		Enemy->OnHit(50);

		AttachToComponent(OtherComp, FAttachmentTransformRules::KeepWorldTransform);

		if (arrowProjectileMovementComponent)
		{
			APlayerCharacter* player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
			player->WeaponComponent1->CrossHair->SetOpacity(1);
			arrowProjectileMovementComponent->StopMovementImmediately();

			arrowProjectileMovementComponent->bSimulationEnabled = false;
			ArrowMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			FTimerHandle visibleTime;
			ArrowMesh->SetRelativeRotation(FRotator(0, 0, 0));

			FTimerDelegate TimerLambda = FTimerDelegate::CreateLambda([this](){SetActive(false);});
			GetWorld()->GetTimerManager().SetTimer(visibleTime, TimerLambda, 10.0f, false);
		}
	}
}

void AArrowActor::SetActive(bool bValue)
{
	ArrowMesh->SetVisibility(bValue);

	if (bValue)
		ArrowMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	else
		ArrowMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
