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
	float MoveForward=0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="PlayerSetting")
	float MoveRight=0.0f;

	FVector Direction;

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



public:
	//�¿� ȸ�� �Է� �Լ�
	void TurnHandler(const struct FInputActionValue& InputValue);
	//���� ȸ�� �Է� �Լ�
	void LookUpHandler(const struct FInputActionValue& InputValue);
	//�̵� �Լ�
	void MoveHandler(const struct FInputActionValue& InputValue);
	//�̵� ��� �Լ�
	void StopHandler(const struct FInputActionValue& InputValue);
	//����
	void JumpHandler(const struct FInputActionValue& InputValue);


};
