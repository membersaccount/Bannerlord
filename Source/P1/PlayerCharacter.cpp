// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "WeaponComponent.h"
#include "WeaponActor.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocationAndRotation(FVector(20, 0, 100),FRotator(-35,0,0));
	SpringArmComp->TargetArmLength = 130;
	SpringArmComp->bUsePawnControlRotation = false;

	Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Cam->SetupAttachment(SpringArmComp);
	Cam->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	//웨폰 컴포넌트 생성
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();


	FName WeaponSocket(TEXT("hand_rSocket"));
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	AWeaponActor* CurWeapon = GetWorld()->SpawnActor<AWeaponActor>(
		WeaponActorFactory,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		SpawnParams
	);

	if (nullptr != CurWeapon) {
		CurWeapon->SetOwner(this);
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
		WeaponComponent->CurrentWeapon = CurWeapon;
	}


	if (auto* playerContoller = Cast<APlayerController>(GetController())) {
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			playerContoller->GetLocalPlayer());
		if (subSys)subSys->AddMappingContext(IMC_Player, 0);

	}

	Anim = GetMesh()->GetAnimInstance();
	Anim->OnMontageStarted.AddDynamic(this, &APlayerCharacter::OnMyMontageStarted);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	direction = FTransform(GetControlRotation()).TransformVector(direction);
	AddMovementInput(direction);
	direction = FVector::ZeroVector;
	ForwardSpeed = FVector::DotProduct(this->GetVelocity(), GetActorForwardVector());
	RightSpeed = FVector::DotProduct(this->GetVelocity(), GetActorRightVector());


}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (auto playerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		playerInput->BindAction(IA_LookUp, ETriggerEvent::Triggered, this, &APlayerCharacter::lookUpHandler);
		playerInput->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &APlayerCharacter::turnHandler);
		playerInput->BindAction(IA_PlayerMove, ETriggerEvent::Triggered, this, &APlayerCharacter::moveHandler);
		playerInput->BindAction(IA_PlayerMove, ETriggerEvent::Completed, this, &APlayerCharacter::stopHandler);
		playerInput->BindAction(IA_Jump, ETriggerEvent::Started, this, &APlayerCharacter::jumpHandler);
		playerInput->BindAction(IA_NONE, ETriggerEvent::Started, this, &APlayerCharacter::noneWeaponHandler);
		playerInput->BindAction(IA_SPEAR, ETriggerEvent::Started, this, &APlayerCharacter::spearWeaponHandler);
		playerInput->BindAction(IA_BOW, ETriggerEvent::Started, this, &APlayerCharacter::bowWeaponHandler);
		playerInput->BindAction(IA_MouseLeftClick, ETriggerEvent::Started, this, &APlayerCharacter::AttackLPressHandler);

		playerInput->BindAction(IA_MouseLeftClick, ETriggerEvent::Completed, this, &APlayerCharacter::AttackLReleaseHandler);

		playerInput->BindAction(IA_MouseRightClick, ETriggerEvent::Started, this, &APlayerCharacter::AttackRPressHandler);
		playerInput->BindAction(IA_MouseRightClick, ETriggerEvent::Completed, this, &APlayerCharacter::AttackRReleaseHandler);
	}
}

void APlayerCharacter::turnHandler(const struct FInputActionValue& InputValue)
{
	float targetYawSpeed = InputValue.Get<float>();

	// 이전 Yaw 속도와 목표 Yaw 속도를 보간하여 부드럽게 변경

	// 보간된 값으로 컨트롤러 회전 적용
	AddControllerYawInput(FMath::FInterpTo(SpringArmComp->GetRelativeRotation().Yaw, targetYawSpeed, GetWorld()->GetDeltaSeconds(), 20.0f));

}

void APlayerCharacter::lookUpHandler(const struct FInputActionValue& InputValue)
{
	float Value = InputValue.Get<float>();
	if (FMath::IsNearlyZero(Value)) return;

	FRotator CurrentRotation = SpringArmComp->GetRelativeRotation();

	float TargetPitch = FMath::Clamp(CurrentRotation.Pitch + Value, -85.0f, 70.0f);

	// 보간 적용
	float InterpPitch = FMath::FInterpTo(CurrentRotation.Pitch, TargetPitch, GetWorld()->GetDeltaSeconds(), 200.0f);

	CurrentRotation.Pitch = InterpPitch;
	SpringArmComp->SetRelativeRotation(CurrentRotation);


}

void APlayerCharacter::moveHandler(const struct FInputActionValue& InputValue)
{
	FVector2D value = InputValue.Get<FVector2D>();
	direction.X = value.X;
	moveRight = value.X;

	direction.Y = value.Y;
	moveForward = value.Y;
}

void APlayerCharacter::stopHandler(const struct FInputActionValue& InputValue)
{
	moveRight = 0.0f;
	moveForward = 0.0f;
}

void APlayerCharacter::jumpHandler(const struct FInputActionValue& InputValue)
{
	Jump();
}

void APlayerCharacter::noneWeaponHandler(const struct FInputActionValue& InputValue)
{
	WeaponComponent->changeWeaponState(EWeaponState::NONE);
}

void APlayerCharacter::spearWeaponHandler(const struct FInputActionValue& InputValue)
{
	WeaponComponent->changeWeaponState(EWeaponState::SPEAR);
}

void APlayerCharacter::bowWeaponHandler(const struct FInputActionValue& InputValue)
{
	WeaponComponent->changeWeaponState(EWeaponState::BOW);
}

void APlayerCharacter::AttackLPressHandler(const struct FInputActionValue& InputValue)
{
	eChractoerState = ECharacterState::ATTACKING;
	WeaponComponent->attackHandler();

}

void APlayerCharacter::AttackLReleaseHandler(const struct FInputActionValue& InputValue)
{
	eChractoerState = ECharacterState::IDLE;
}

void APlayerCharacter::AttackRPressHandler(const struct FInputActionValue& InputValue)
{
	eChractoerState = ECharacterState::ATTACKING;
	WeaponComponent->guardHandler();

}

void APlayerCharacter::AttackRReleaseHandler(const struct FInputActionValue& InputValue)
{
	eChractoerState = ECharacterState::IDLE;
}

void APlayerCharacter::OnMyMontageStarted(UAnimMontage* Montage)
{
	OnMyPlayMontage(Montage);
}
