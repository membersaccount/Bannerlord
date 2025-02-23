// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponMontageData.h"
#include "../PlayerCharacter.h"
#include "EMouseState.h"
#include "Sound/SoundCue.h"
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
	USceneComponent* SceneComp;
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	UStaticMeshComponent* SpearMesh;
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	UStaticMeshComponent* SwordMesh;
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USkeletalMeshComponent* BowMesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Weapon)
	TSubclassOf<class UDataAsset> MontageDataTable;
	UPROPERTY()
	TMap<EWeaponState, FWeaponMontageData> CachedMontages;
	UAnimMontage* hitMontage;
	UAnimMontage* dieMontage;
	UPROPERTY()
	APlayerCharacter* me;
	EWeaponState weaponStateIn;
	FWeaponMontageData MontageData;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
    class UNiagaraSystem* BloodSplatterNiagara;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundCue* hitSound;

public:
	void playChangeMontage(EWeaponState weaponState);
	void playAttackMontage(EWeaponState weaponState,EMouseState mouseState);
	void playGuardMontage(EWeaponState weaponState,EMouseState mouseState);
	void playHitMontage();
	void playDieMotage();
	void loadWeapon();
	void selectWeapon();
	void showCrossHair(bool isHit);

	UFUNCTION()
	void overlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
