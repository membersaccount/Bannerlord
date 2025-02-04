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

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (auto playerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		playerInput->BindAction(IA_LookUp, ETriggerEvent::Triggered, this, &APlayerCharacter::LookUp);
		playerInput->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &APlayerCharacter::Turn);
	}

}

void APlayerCharacter::Turn(const struct FInputActionValue& InputValue)
{
	float TargetYawSpeed = InputValue.Get<float>();

	// ���� Yaw �ӵ��� ��ǥ Yaw �ӵ��� �����Ͽ� �ε巴�� ����

	// ������ ������ ��Ʈ�ѷ� ȸ�� ����
	AddControllerYawInput(FMath::FInterpTo(SpringArmComp->GetRelativeRotation().Yaw, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 20.0f));
	/*
	// ��Ʈ�ѷ��� ���� Yaw �� ��������
	float CurrentYaw = GetControlRotation().Yaw;

	// ĳ������ ���� Yaw �� ��������
	float CharacterYaw = GetActorRotation().Yaw;

	// �� ������ ���� ���
	float DeltaYaw = FMath::FindDeltaAngleDegrees(CharacterYaw, CurrentYaw);

	// Ư�� ���� �̻� ���̰� ���� �ε巴�� ȸ��
	if (FMath::Abs(DeltaYaw) > 60.0f)
	{
		FRotator NewRotation = FRotator(0.0f, CurrentYaw, 0.0f);
		FRotator SmoothedRotation = FMath::RInterpTo(GetActorRotation(), NewRotation, GetWorld()->GetDeltaSeconds(), 5.0f);
		SetActorRotation(SmoothedRotation);
	}
	*/

}

void APlayerCharacter::LookUp(const struct FInputActionValue& InputValue)
{
	float Value = InputValue.Get<float>();
	if (FMath::IsNearlyZero(Value)) return;  // �Է��� �̹��ϸ� ����

	// ���� Pitch �� ��������
	FRotator CurrentRotation = SpringArmComp->GetRelativeRotation();

	// ��ǥ ȸ�� �� ����
	float TargetPitch = FMath::Clamp(CurrentRotation.Pitch + Value, -85.0f, 70.0f);

	// �ε巯�� ���� ����
	float InterpPitch = FMath::FInterpTo(CurrentRotation.Pitch, TargetPitch, GetWorld()->GetDeltaSeconds(), 20.0f);

	// ���ο� ȸ�� ����
	CurrentRotation.Pitch = InterpPitch;
	SpringArmComp->SetRelativeRotation(CurrentRotation);


}

