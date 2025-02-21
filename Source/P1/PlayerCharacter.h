// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ECharacterState.h"
#include "ArrowProjectileMovementComponent.h"
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

private:
	float HP = 100;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerSetting")
	float moveForward = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerSetting")
	float moveRight = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerSetting")
	float ForwardSpeed= 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerSetting")
	float RightSpeed = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerSetting")
	class UArrowComponent* ArrowComp;

	int32 MaxArrowCnt = 20;

	TArray<class AArrowActor*> Magazine;

	FVector direction;

	bool isAttack = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "playerState")
	ECharacterState eChractoerState = ECharacterState::IDLE;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	UStaticMeshComponent* SpearMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<class ADefaultActor> QuiverClass;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USkeletalMeshComponent* BowMesh;

	AArrowActor* spawnedArrow;

	AArrowActor* arrow;

	bool isHit = false;

	bool bDead = false;


public:
	class AWeaponActor* CurWeapon;
	UPROPERTY(EditAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere, Category = "Camera")
	class UCameraComponent* Cam;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_Player;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_LookUp;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Turn;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_PlayerMove;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Jump;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_NONE;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_SPEAR;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_BOW;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_MouseLeftClick;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_MouseRightClick;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Sword;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Order_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Order_Attact;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponComponent")
	class UWeaponComponent* WeaponComponent1;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AWeaponActor> WeaponActorFactory;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AArrowActor> arrowActor;

	UPROPERTY()
	class UAnimInstance* Anim;

		UPROPERTY(VisibleAnywhere, Category = "Movement")
	UArrowProjectileMovementComponent* arrowProjectileMovementComponent;


public:

	float getHP() const;

	void setHP(int InValue);
	//�¿� ȸ�� �Է� �Լ�
	void turnHandler(const struct FInputActionValue& InputValue);
	//���� ȸ�� �Է� �Լ�
	void lookUpHandler(const struct FInputActionValue& InputValue);
	//�̵� �Լ�
	void moveHandler(const struct FInputActionValue& InputValue);
	//�̵� ��� �Լ�
	void stopHandler(const struct FInputActionValue& InputValue);
	//����
	void jumpHandler(const struct FInputActionValue& InputValue);

	//ZŰ �Է�
	void noneWeaponHandler(const struct FInputActionValue& InputValue);
	//XŰ�Է�
	void spearWeaponHandler(const struct FInputActionValue& InputValue);
	//CŰ�Է�
	void bowWeaponHandler(const struct FInputActionValue& InputValue);
	//VŰ�Է�
	void swordWeaponHandler(const struct FInputActionValue& InputValue);

	//���콺 ��Ŭ��
	void AttackLPressHandler(const struct FInputActionValue& InputValue);

	void AttackLReleaseHandler(const struct FInputActionValue& InputValue);

	//���콺 ��Ŭ��
	void AttackRPressHandler(const struct FInputActionValue& InputValue);

	void AttackRReleaseHandler(const struct FInputActionValue& InputValue);

	//���� ���� ��ġ ����
	void weaponSoketChange(bool isChange);

	void setArrowVisible();

	UFUNCTION()
	void OnMyMontageStarted(class UAnimMontage* Montage);
	UFUNCTION(BluePrintCallable, BlueprintImplementableEvent)
	void OnMyPlayMontage(class UAnimMontage* Montage);

	void spawnArrow();

	void MakeBullet();

	void arrowShotHandler();

	void hitHandler(float enenmyDamage);

	void OrderMoveHander();

	void OrderAttackHander();



};
