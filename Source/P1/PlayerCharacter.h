// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ECharacterState.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponComponent")
	class UWeaponComponent* WeaponComponent1;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AWeaponActor> WeaponActorFactory;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AArrowActor> arrowActor;

	AArrowActor* arrow;

	UPROPERTY()
	class UAnimInstance* Anim;


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
	void noneWeaponHandler(const struct FInputActionValue& InputValue);
	//X키입력
	void spearWeaponHandler(const struct FInputActionValue& InputValue);
	//C키입력
	void bowWeaponHandler(const struct FInputActionValue& InputValue);
	//V키입력
	void swordWeaponHandler(const struct FInputActionValue& InputValue);

	//마우스 좌클릭
	void AttackLPressHandler(const struct FInputActionValue& InputValue);

	void AttackLReleaseHandler(const struct FInputActionValue& InputValue);

	//마우스 우클릭
	void AttackRPressHandler(const struct FInputActionValue& InputValue);

	void AttackRReleaseHandler(const struct FInputActionValue& InputValue);

	//무기 소켓 위치 변경
	void weaponSoketChange(bool isChange);

	void setArrowVisible();

	UFUNCTION()
	void OnMyMontageStarted(class UAnimMontage* Montage);
	UFUNCTION(BluePrintCallable, BlueprintImplementableEvent)
	void OnMyPlayMontage(class UAnimMontage* Montage);
	void spawnArrow();
};
