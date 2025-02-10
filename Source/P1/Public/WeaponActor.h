// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponMontageData.h"
#include "WeaponActor.generated.h"

UCLASS()
class P1_API AWeaponActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	UStaticMeshComponent* Weapon;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Weapon)
	TSubclassOf<class UDataTable> MontageDataTable;

	TArray<class UAnimMontage*> attackMontages;

	UAnimMontage* hitMontage;

	UAnimMontage* dieMontage;

	TMap<EWeaponState, FWeaponMontageData> weaponMontageDataMap;

public:
	void playChangeMontage(EWeaponState weaponState);
	void playAttackMontage();
	void playHitMontage();
	void playDieMotage();







};
