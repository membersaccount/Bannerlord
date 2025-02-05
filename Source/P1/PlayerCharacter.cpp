// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"

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

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (auto* playerContoller = Cast<APlayerController>(GetController())) {
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			playerContoller->GetLocalPlayer());
		if (subSys)subSys->AddMappingContext(IMC_Player, 0);


	}

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Direction = FTransform(GetControlRotation()).TransformVector(Direction);
	AddMovementInput(Direction);
	Direction = FVector::ZeroVector;
	//MoveForward = 0.0f;
	//MoveRight = 0.0f;

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (auto playerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		playerInput->BindAction(IA_LookUp, ETriggerEvent::Triggered, this, &APlayerCharacter::LookUpHandler);
		playerInput->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &APlayerCharacter::TurnHandler);
		playerInput->BindAction(IA_PlayerMove, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveHandler);
		playerInput->BindAction(IA_PlayerMove, ETriggerEvent::Completed, this, &APlayerCharacter::StopHandler);
		playerInput->BindAction(IA_Jump, ETriggerEvent::Started, this, &APlayerCharacter::JumpHandler);
	}

}

void APlayerCharacter::TurnHandler(const struct FInputActionValue& InputValue)
{
	float TargetYawSpeed = InputValue.Get<float>();

	// 이전 Yaw 속도와 목표 Yaw 속도를 보간하여 부드럽게 변경

	// 보간된 값으로 컨트롤러 회전 적용
	AddControllerYawInput(FMath::FInterpTo(SpringArmComp->GetRelativeRotation().Yaw, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 20.0f));

}

void APlayerCharacter::LookUpHandler(const struct FInputActionValue& InputValue)
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

void APlayerCharacter::MoveHandler(const struct FInputActionValue& InputValue)
{
	FVector2D value = InputValue.Get<FVector2D>();
	Direction.X = value.X;
	MoveRight = value.X;

	Direction.Y = value.Y;
	MoveForward = value.Y;
}

void APlayerCharacter::StopHandler(const struct FInputActionValue& InputValue)
{
	MoveRight = 0.0f;
	MoveForward = 0.0f;
}

void APlayerCharacter::JumpHandler(const struct FInputActionValue& InputValue)
{
	Jump();
}

