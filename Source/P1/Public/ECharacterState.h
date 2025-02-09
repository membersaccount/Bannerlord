// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
UENUM()
enum class  ECharacterState:uint8
{
	IDLE,
	ATTACK,
	HIT,
	DIE,
	ATTACKING,
	GAURD,
};
