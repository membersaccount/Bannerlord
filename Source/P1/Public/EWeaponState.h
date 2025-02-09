// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
UENUM(BlueprintType)
enum class EWeaponState:uint8
{
	NONE UMETA(DisplayName="NONE"),
	SPEAR UMETA(DisplayName = "SPEAR"),
	SWORD UMETA(DisplayName = "SWORD"),
	BOW UMETA(DisplayName = "BOW"),
	MAX
};
