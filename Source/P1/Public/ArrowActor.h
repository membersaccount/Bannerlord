// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArrowActor.generated.h"

UCLASS()
class P1_API AArrowActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AArrowActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UStaticMeshComponent* ArrowMesh;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USceneComponent* SceneComp;

	//void targetSetLocation(FVector TargetLocation);

	APlayerController* PC;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	class UArrowProjectileMovementComponent* arrowProjectileMovementComponent;


	FVector PreviousLocation;
	UFUNCTION()
		void overlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
