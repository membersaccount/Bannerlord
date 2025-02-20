// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EWeaponState.h"
#include "../PlayerCharacter.h"
#include "EMouseState.h"
#include "Blueprint/UserWidget.h"
#include "WeaponComponent.generated.h"

/** foward declarations */
class UImage;

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
	//위젯 애니메이션 저장
	UPROPERTY()
    UWidgetAnimation* AimAnimation;

	UImage* ArrowT;
	UImage* ArrowB;
	UImage* ArrowR;
	UImage* ArrowL;

	UImage* AimT;
	UImage* AimB;
	UImage* AimR;
	UImage* AimL;

	UImage* CrossHair;
	UImage* HPui;
	UImage* spearImg;
	UImage* sowrdImg;
	UImage* bowImg;




	UPROPERTY(EditDefaultsOnly)
	class UplayerWidget* widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widge")
	TSubclassOf<class UplayerWidget>widgetFactory;

	float mouseY = 0.0f;
	float mouseX = 0.0f;
	FVector2D lastMousePosition;
	FVector2D currentMousePosition;
	EMouseState EmouseDirection=EMouseState::NONE;

	EMouseState GetMouseDirection() const { return EmouseDirection; }
	float AimWidgetPosition;



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

	void showAim(bool isAim);
	void showArrow();

	void getWidgetPosition();
};
