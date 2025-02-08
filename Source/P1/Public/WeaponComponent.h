// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EWeaponState.h"
#include "WeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class P1_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



////////////////////////////////////////////////////
//변수선언
public:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="FMS")
	EWeaponState weaponState=EWeaponState::NONE;


//함수 선언
public:
	void changeWeaponState(enum EWeaponState weaponState);
	//기본 애니메이션 변경
	void changeWeaponAnamation();

	void attackUpHandler();

	void attackDownHandler();

	void attackRightHandler();

	void attackLeftHandler();

	void hitActionHandler();

	void dieActionHandler();

};
