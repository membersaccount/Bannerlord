// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class P1_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category ="PlayerSetting")
	float moveForward=0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="PlayerSetting")
	float moveRight=0.0f;

	FVector direction;

public:
	UPROPERTY(EditAnywhere,Category="Camera")
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere,Category="Camera")
	class UCameraComponent* Cam;

	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputMappingContext* IMC_Player;

	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* IA_LookUp;

	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* IA_Turn;

	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* IA_PlayerMove;

	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* IA_Jump;
		UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* IA_NONE;
		UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* IA_SPEAR;
		UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* IA_BOW;


	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="WeaponComponent")
	class UWeaponComponent* WeaponComponent;



public:


	//좌우 회전 입력 함수
	void turnHandler(const struct FInputActionValue& InputValue);
	//상하 회전 입력 함수
	void lookUpHandler(const struct FInputActionValue& InputValue);
	//이동 함수
	void moveHandler(const struct FInputActionValue& InputValue);
	//이동 취소 함수
	void stopHandler(const struct FInputActionValue& InputValue);
	//점프
	void jumpHandler(const struct FInputActionValue& InputValue);

	//Z키 입력
	void NoneWeaponHandler(const struct FInputActionValue& InputValue);
	//X키입력
	void SpearWeaponHandler(const struct FInputActionValue& InputValue);
	//C키입력
	void BowWeaponHandler(const struct FInputActionValue& InputValue);



};
