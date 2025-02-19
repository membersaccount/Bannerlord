// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "WeaponComponent.h"
#include "WeaponActor.h"
#include "ArrowActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"
#include "DefaultActor.h"
#include "playerWidget.h"
#include "Animation/WidgetAnimation.h"
#include "Components/ArrowComponent.h"
#include "ArrowProjectileMovementComponent.h"
#include "Components/Image.h"
#include "EWeaponState.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocationAndRotation(FVector(20, 0, 100), FRotator(-35, 0, 0));
	SpringArmComp->TargetArmLength = 130;
	SpringArmComp->bUsePawnControlRotation = false;

	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	ArrowComp->SetupAttachment(SpringArmComp);

	Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Cam->SetupAttachment(SpringArmComp);
	Cam->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	SpearMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON1"));
	BowMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON2"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> Spear_Weapon(TEXT("/Script/Engine.StaticMesh'/Game/LHW/Assets/Models/Mesh/Spear_Spear_LOD0.Spear_Spear_LOD0'"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> Bow_Weapon(TEXT("/ Script / Engine.SkeletalMesh'/Game/LHW/ArcherAnimsetPro/Meshes/Bow/SK_Bow.SK_Bow'"));

	if (Spear_Weapon.Succeeded() && Bow_Weapon.Succeeded()) {

		SpearMesh->SetStaticMesh(Spear_Weapon.Object);
		BowMesh->SetSkeletalMesh(Bow_Weapon.Object);

		SpearMesh->SetupAttachment(GetMesh(), TEXT("spear"));
		BowMesh->SetupAttachment(GetMesh(), TEXT("bow"));
	}

WeaponComponent1 = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent1"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	arrow=Cast<AArrowActor>(arrowActor);


	FName WeaponSocket(TEXT("hand_rSocket"));
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	CurWeapon = GetWorld()->SpawnActor<AWeaponActor>(
		WeaponActorFactory,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		SpawnParams
	);

	if (nullptr != CurWeapon) {
		CurWeapon->SetOwner(this);
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
		WeaponComponent1->CurrentWeapon = CurWeapon;
	}


	if (auto* playerContoller = Cast<APlayerController>(GetController())) {
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			playerContoller->GetLocalPlayer());
		if (subSys)subSys->AddMappingContext(IMC_Player, 0);

	}

	Anim = GetMesh()->GetAnimInstance();
	Anim->OnMontageStarted.AddDynamic(this, &APlayerCharacter::OnMyMontageStarted);

	ADefaultActor* A =Cast<ADefaultActor>(GetWorld()->SpawnActor(QuiverClass));
	A->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("quiver"));
	A->SetActorEnableCollision(false);
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
		playerInput->BindAction(IA_Sword, ETriggerEvent::Started, this, &APlayerCharacter::swordWeaponHandler);


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
	WeaponComponent1->changeWeaponState(EWeaponState::NONE);
}

void APlayerCharacter::spearWeaponHandler(const struct FInputActionValue& InputValue)
{
	weaponSoketChange(false);
	WeaponComponent1->changeWeaponState(EWeaponState::SPEAR);
}

void APlayerCharacter::bowWeaponHandler(const struct FInputActionValue& InputValue)
{
	weaponSoketChange(true);
	WeaponComponent1->changeWeaponState(EWeaponState::BOW);

}

void APlayerCharacter::swordWeaponHandler(const struct FInputActionValue& InputValue)
{
	weaponSoketChange(false);
	WeaponComponent1->changeWeaponState(EWeaponState::SWORD);

}

void APlayerCharacter::AttackLPressHandler(const struct FInputActionValue& InputValue)
{
	if (isAttack) return;
	isAttack = true;
	eChractoerState = ECharacterState::ATTACKING;
	WeaponComponent1->attackHandler();
	if (WeaponComponent1->widget->AimAnimation) {
		WeaponComponent1->widget->AimPlayAnimation(true);
	}

}

void APlayerCharacter::AttackLReleaseHandler(const struct FInputActionValue& InputValue)
{
	eChractoerState = ECharacterState::IDLE;
	if (WeaponComponent1->widget->AimAnimation) {
		WeaponComponent1->widget->AimPlayAnimation(false);
		if (WeaponComponent1->weaponState == EWeaponState::BOW) {
			isAttack = false;
			Anim->Montage_Play(CurWeapon->MontageData.BowIdle);
			arrowShotHandler();
		}
	}
}

void APlayerCharacter::AttackRPressHandler(const struct FInputActionValue& InputValue)
{
	eChractoerState = ECharacterState::ATTACKING;
	WeaponComponent1->guardHandler();

}

void APlayerCharacter::AttackRReleaseHandler(const struct FInputActionValue& InputValue)
{
	eChractoerState = ECharacterState::IDLE;
}

void APlayerCharacter::weaponSoketChange(bool isChange)
{
	if(isChange){
	if (nullptr != CurWeapon) {
		CurWeapon->SetOwner(this);
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName(TEXT("hand_lSocket")));
		WeaponComponent1->CurrentWeapon = CurWeapon;
	}
	}
	else {
		if (nullptr != CurWeapon) {
			CurWeapon->SetOwner(this);
			CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName(TEXT("hand_RSocket")));
			WeaponComponent1->CurrentWeapon = CurWeapon;
		}
	}
}
void APlayerCharacter::OnMyMontageStarted(UAnimMontage* Montage)
{
	OnMyPlayMontage(Montage);
}

void APlayerCharacter::spawnArrow()
{
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	FTransform arrowPos = GetMesh()->GetSocketTransform(TEXT("arrow"));

	spawnedArrow = GetWorld()->SpawnActor<AArrowActor>(arrowActor, arrowPos, spawnParams);


	if (spawnedArrow)
	{
		spawnedArrow->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("arrow"));

		arrowProjectileMovementComponent = spawnedArrow->FindComponentByClass<UArrowProjectileMovementComponent>();
	}
}

void APlayerCharacter::arrowShotHandler()
{
	if (arrowProjectileMovementComponent)
	{
		spawnedArrow->ArrowMesh->SetRelativeRotation(FRotator(0, -90, 0));
		spawnedArrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		FVector Start, End;
		FRotator CameraRotation;
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController)
		{
			PlayerController->GetPlayerViewPoint(Start, CameraRotation);
			FVector ForwardVector = CameraRotation.Vector();
			//
			Start += FVector(0.0f,
				FMath::RandRange(-WeaponComponent1->AimWidgetPosition, WeaponComponent1->AimWidgetPosition),
				FMath::RandRange(-WeaponComponent1->AimWidgetPosition, WeaponComponent1->AimWidgetPosition));
			//
			End = Start + (ForwardVector * 3000.f); // 50m = 5000cm


			bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

			FVector TargetLocation;
			if (bHit)
			{
				TargetLocation = HitResult.Location;
			}
			else
			{
				TargetLocation = End;
			}

			FVector LaunchDirection = (TargetLocation - GetActorLocation()).GetSafeNormal();
			arrowProjectileMovementComponent->shotHandler(TargetLocation);

			DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);
			DrawDebugPoint(GetWorld(), TargetLocation, 10.0f, FColor::Red, false, 1.0f);
		}
	}
}
