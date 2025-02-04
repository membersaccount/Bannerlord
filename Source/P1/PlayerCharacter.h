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

public:
	//좌우 회전 입력 처리
	void Turn(const struct FInputActionValue& InputValue);
	//상하 회전 입력 처리
	void LookUp(const struct FInputActionValue& InputValue);

};
