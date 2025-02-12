// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EWeaponState.h"
#include "../PlayerCharacter.h"
#include "EMouseState.h"
#include "Blueprint/UserWidget.h"
#include "WeaponComponent.generated.h"



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FMS")
	EWeaponState weaponState = EWeaponState::NONE;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	class AWeaponActor* CurrentWeapon;

	class UImage* ArrowT;
	class UImage* ArrowB;
	class UImage* ArrowR;
	class UImage* ArrowL;

	UPROPERTY(EditDefaultsOnly)
	class UUserWidget* widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widge")
	TSubclassOf<class UUserWidget>widgetFactory;

	float mouseY = 0.0f;
	float mouseX = 0.0f;
	FVector2D lastMousePosition;
	FVector2D currentMousePosition;
	EMouseState EmouseDirection=EMouseState::NONE;

	EMouseState GetMouseDirection() const { return EmouseDirection; }



	//함수 선언
public:
	void changeWeaponState(EWeaponState weaponState);
	//기본 애니메이션 변경
	void changeWeaponAnamation();

	void attackHandler();

	void guardHandler();

	void hitActionHandler();

	void dieActionHandler();

	void updateMouseDirection();



};
