// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponMontageData.h"
#include "EWeaponState.h"
#include "WeaponMontageDataAsset.generated.h"
/**
 *
 */
UCLASS()
class P1_API UWeaponMontageDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EWeaponState, FWeaponMontageData> WeaponMontageMap;

};
