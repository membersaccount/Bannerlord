// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IWeaponInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class P1_API IIWeaponInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void attackUp() = 0;
	virtual void attackDown() = 0;
	virtual void attackRight() = 0;
	virtual void attackLeft() = 0;
	virtual void hitAction() = 0;
	virtual void dieAction() = 0;
};
