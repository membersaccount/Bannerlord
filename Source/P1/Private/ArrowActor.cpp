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
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Classes/NiagaraSystem.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
#include "KillLogWidget.h"
#include "WeaponActor.h"

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

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> TempNiagaraSystem(
		TEXT("/Script/Niagara.NiagaraSystem'/Game/LHW/Assets/Blood_VFX_Pack/Particles/Systems/P_BigSplash_Hit.P_BigSplash_Hit'"));

	if (TempNiagaraSystem.Succeeded())
	{
		BloodSplatterNiagara = TempNiagaraSystem.Object;
	}

}

// Called when the game starts or when spawned
void AArrowActor::BeginPlay()
{
	Super::BeginPlay();
	PreviousLocation = arrowProjectileMovementComponent->UpdatedComponent->GetComponentLocation();
	player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());

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
			int enemystate=Enemy->OnHit(player->CurWeapon->MontageData.damage);

		if (BloodSplatterNiagara)
		{
			if (enemystate == 0) {
				FText KillMessage = FText::Format(NSLOCTEXT("KillLog", "KillMessage", "피해입힘 {0} "), player->CurWeapon->MontageData.damage);
				player->widget->AddKillLogEntry(KillMessage, enemystate);
			}
			else if (enemystate == 1) {
				FText KillMessage = FText::Format(NSLOCTEXT("KillLog", "KillMessage", "{0} 제국 민병대 창병"), player->CurWeapon->MontageData.damage);
				player->widget->AddKillLogEntry(KillMessage, enemystate);
			}


			FVector HitLocation = SweepResult.ImpactPoint;
			auto BloodComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodSplatterNiagara, OtherActor->GetActorLocation());
		}

		AttachToComponent(OtherComp, FAttachmentTransformRules::KeepWorldTransform);

		if (arrowProjectileMovementComponent)
		{
			player->WeaponComponent1->CrossHair->SetOpacity(1);
			FTimerHandle visibleTime1;

			FTimerDelegate showLamda = FTimerDelegate::CreateLambda([this]() {	showCrossHair(); });
			GetWorld()->GetTimerManager().SetTimer(visibleTime1, showLamda,0.5f, false);
			arrowProjectileMovementComponent->StopMovementImmediately();

			arrowProjectileMovementComponent->bSimulationEnabled = false;
			ArrowMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			ArrowMesh->SetRelativeRotation(FRotator(0, 0, 0));
			FTimerHandle visibleTime2;

			FTimerDelegate TimerLambda = FTimerDelegate::CreateLambda([this](){SetActive(false);});
			GetWorld()->GetTimerManager().SetTimer(visibleTime2, TimerLambda, 10.0f, false);
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

void AArrowActor::showCrossHair()
{
	player->WeaponComponent1->CrossHair->SetOpacity(0);
}
