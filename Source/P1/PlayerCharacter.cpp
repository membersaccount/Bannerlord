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
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Components/ProgressBar.h"
#include "KillLogWidget.h"
#include "Components/VerticalBox.h"
#include "GameModes/MBBattleGameMode.h"
#include "Kismet/GameplayStatics.h"
typedef Enums::Player::Order::Formation Formation;
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

	gameMode = Cast<AMBBattleGameMode>(UGameplayStatics::GetGameMode(GetWorld()->GetAuthGameMode()));
	widget = CreateWidget<UKillLogWidget>(GetWorld(), widgetFactory);
	widget->AddToViewport();
	widget->MaxEntries = 10;

	for (int32 i= 0; i < MaxArrowCnt; ++i) {
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AArrowActor* arrows = GetWorld()->SpawnActor<AArrowActor>(arrowActor, params);
		arrows->SetActive(false);
		Magazine.Add(arrows);
	}


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
		switch (eChractoerState)
		{
		case ECharacterState::IDLE:
			break;
		case ECharacterState::ATTACK:
			break;
		case ECharacterState::HIT:
			break;
		case ECharacterState::DIE:
			break;
		case ECharacterState::ATTACKING:
			break;
		case ECharacterState::GAURD:
			break;
		default:
			break;
		}

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
			playerInput->BindAction(IA_Order_Move, ETriggerEvent::Started, this, &APlayerCharacter::OrderMoveHander);
			playerInput->BindAction(IA_Order_Attact, ETriggerEvent::Started, this, &APlayerCharacter::OrderAttackHander);
			playerInput->BindAction(IA_Order3, ETriggerEvent::Started, this, &APlayerCharacter::orderHander3);
			playerInput->BindAction(IA_Order4, ETriggerEvent::Started, this, &APlayerCharacter::orderHander4);
			playerInput->BindAction(IA_Order5, ETriggerEvent::Started, this, &APlayerCharacter::orderHander5);
			playerInput->BindAction(IA_Order6, ETriggerEvent::Started, this, &APlayerCharacter::orderHander6);
			playerInput->BindAction(IA_Order7, ETriggerEvent::Started, this, &APlayerCharacter::orderHander7);
			playerInput->BindAction(IA_Order8, ETriggerEvent::Started, this, &APlayerCharacter::enemyOrderHander);
			playerInput->BindAction(IA_Horn, ETriggerEvent::Started, this, &APlayerCharacter::soundPlayer);
	}
}

float APlayerCharacter::getHP() const
{
	return HP;
}

void APlayerCharacter::setHP(int InValue)
{
	HP = InValue;
}

void APlayerCharacter::turnHandler(const struct FInputActionValue& InputValue)
{
	if (isHit) return;

	float targetYawSpeed = InputValue.Get<float>();

	// 이전 Yaw 속도와 목표 Yaw 속도를 보간하여 부드럽게 변경

	// 보간된 값으로 컨트롤러 회전 적용
	AddControllerYawInput(FMath::FInterpTo(SpringArmComp->GetRelativeRotation().Yaw, targetYawSpeed, GetWorld()->GetDeltaSeconds(), 20.0f));

}

void APlayerCharacter::lookUpHandler(const struct FInputActionValue& InputValue)
{
	if (isHit) return;

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
	//hitHandler(50.0f);


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
	if (isAttack) return; 	if (isHit) return; if (bDead)return;
	isAttack = true;
	eChractoerState = ECharacterState::ATTACKING;
	WeaponComponent1->attackHandler();
	if (WeaponComponent1->widget->AimAnimation) {
		WeaponComponent1->widget->AimPlayAnimation(true);
	}

}

void APlayerCharacter::AttackLReleaseHandler(const struct FInputActionValue& InputValue)
{
	if (isHit) {
		arrowShotHandler();
	}
	else {
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

}

void APlayerCharacter::AttackRPressHandler(const struct FInputActionValue& InputValue)
{
	if (isHit) return; if (bDead)return;

	eChractoerState = ECharacterState::ATTACKING;
	WeaponComponent1->guardHandler();

}

void APlayerCharacter::AttackRReleaseHandler(const struct FInputActionValue& InputValue)
{
	if (isHit) return; if (bDead)return;

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
	if (isHit) return; if (bDead)return;

	MakeBullet();

}

void APlayerCharacter::MakeBullet()
{
	bool FindResult = false;
	for (int32 i = 0; i < Magazine.Num(); ++i) {
		if (!Magazine[i]->ArrowMesh->GetVisibleFlag()) {
			FTransform arrowPos = GetMesh()->GetSocketTransform(TEXT("arrow"));
			FindResult = true;
			spawnedArrow = Magazine[i];

			Magazine[i]->SetActive(true);
			Magazine[i]->SetActorTransform(arrowPos);


			Magazine[i]->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("arrow"));

			arrowProjectileMovementComponent = Magazine[i]->FindComponentByClass<UArrowProjectileMovementComponent>();
			break;
		}
	}

}

void APlayerCharacter::arrowShotHandler()
{
	FVector TargetLocation = GetActorLocation();
	if (isHit) {
		spawnedArrow->SetActive(false);
		arrowProjectileMovementComponent->bSimulationEnabled = true;
		return;
	}
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

void APlayerCharacter::hitHandler(float enenmyDamage)
{
	if (getHP() <= 0) {
		isHit = true;
		GetCharacterMovement()->SetMovementMode(MOVE_None);
		eChractoerState = ECharacterState::DIE;
		Anim->Montage_Play(CurWeapon->MontageData.DieMontage);
	}
	if (getHP() > 0) {
		isHit = true;
		GetCharacterMovement()->SetMovementMode(MOVE_None);
		setHP(getHP() - enenmyDamage);
		WeaponComponent1->widget->HPUI->SetPercent(getHP() / 100);
		eChractoerState = ECharacterState::HIT;
		Anim->Montage_Play(CurWeapon->MontageData.HitMontage);

		FTimerHandle visibleTime;

		if (!bDead)
		{
			FTimerDelegate TimerLambda = FTimerDelegate::CreateLambda([this]() {  isAttack = false; 	GetCharacterMovement()->SetMovementMode(MOVE_Walking); isHit = false; eChractoerState = ECharacterState::IDLE; });
			GetWorld()->GetTimerManager().SetTimer(visibleTime, TimerLambda, 2.0f, false);
		}
	}


}

void APlayerCharacter::OrderMoveHander()
{
	if (WeaponComponent1->weaponState == EWeaponState::NONE) {
		UGameplayStatics::PlaySoundAtLocation(this, moveSound, GetActorLocation());

		FRotator CameraRotation;

		APlayerController* PlayerController = Cast<APlayerController>(GetController());

		FVector Start = GetActorLocation(); // 시작 지점 (캐릭터 위치)
		PlayerController->GetPlayerViewPoint(Start, CameraRotation);
		FVector ForwardVector = CameraRotation.Vector();
		FVector End = Start + (ForwardVector * 1000.0f); // 1000cm(10m) 거리로 쏨c

		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this); // 자기 자신은 무시

		// 라인트레이스 실행
		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult, Start, End, ECC_Visibility, CollisionParams);
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, true, 2.0f, 0, 2.0f);

		if (bHit) // 충돌이 발생했을 경우
		{
			FVector HitLocation = HitResult.ImpactPoint; // 충돌한 위치 가져오기
			UE_LOG(LogTemp, Warning, TEXT("Hit Location: %s"), *HitLocation.ToString());

			// 디버그 선을 그려 확인하기
			DrawDebugLine(GetWorld(), Start, HitLocation, FColor::Red, true, 2.0f, 0, 2.0f);
			Anim->Montage_Play(WeaponComponent1->CurrentWeapon->MontageData.BowAim);
			gameMode->OrderPlayerTeam(&gameMode->CharacterStateManager.ManagerOrderMoveLocation, Formation::Default, HitLocation);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Hit"));
			Anim->Montage_Play(WeaponComponent1->CurrentWeapon->MontageData.BowAim);
			gameMode->OrderPlayerTeam(&gameMode->CharacterStateManager.ManagerOrderMoveLocation, Formation::Default, End);
		}

	}
}

void APlayerCharacter::OrderAttackHander()
{

	if (WeaponComponent1->weaponState == EWeaponState::NONE) {
		UGameplayStatics::PlaySoundAtLocation(this, attackSound, GetActorLocation());

		Anim->Montage_Play(WeaponComponent1->CurrentWeapon->MontageData.BowAim);
		gameMode->OrderPlayerTeam(&gameMode->CharacterStateManager.ManagerOrderEngageBattle);
	}
}

void APlayerCharacter::UiVisibleHandler()
{
	WeaponComponent1->widget->SetVisibility(ESlateVisibility::Visible);
}

void APlayerCharacter::orderHander3()
{
	if (WeaponComponent1->weaponState == EWeaponState::NONE) {
		UGameplayStatics::PlaySoundAtLocation(this, attackSound, GetActorLocation());

		Anim->Montage_Play(WeaponComponent1->CurrentWeapon->MontageData.BowAim);
		gameMode->OrderPlayerTeam(&gameMode->CharacterStateManager.ManagerOrderEngageBattle);
	}
}

void APlayerCharacter::orderHander4()
{
	if (WeaponComponent1->weaponState == EWeaponState::NONE) {
		UGameplayStatics::PlaySoundAtLocation(this, orderSound, GetActorLocation());

		Anim->Montage_Play(WeaponComponent1->CurrentWeapon->MontageData.BowAim);
		gameMode->OrderPlayerTeam(&gameMode->CharacterStateManager.ManagerOrderMakeFormation,Formation::Default);
	}
}

void APlayerCharacter::orderHander5()
{
	if (WeaponComponent1->weaponState == EWeaponState::NONE) {
		UGameplayStatics::PlaySoundAtLocation(this, orderSound, GetActorLocation());

		Anim->Montage_Play(WeaponComponent1->CurrentWeapon->MontageData.BowAim);
		gameMode->OrderPlayerTeam(&gameMode->CharacterStateManager.ManagerOrderMakeFormation, Formation::Shield);
	}
}

void APlayerCharacter::orderHander6()
{
	if (WeaponComponent1->weaponState == EWeaponState::NONE) {
		UGameplayStatics::PlaySoundAtLocation(this, orderSound, GetActorLocation());

		Anim->Montage_Play(WeaponComponent1->CurrentWeapon->MontageData.BowAim);
		gameMode->OrderPlayerTeam(&gameMode->CharacterStateManager.ManagerOrderMakeFormation, Formation::Spread);
	}
}

void APlayerCharacter::orderHander7()
{
	if (WeaponComponent1->weaponState == EWeaponState::NONE) {
		UGameplayStatics::PlaySoundAtLocation(this, orderSound, GetActorLocation());

		Anim->Montage_Play(WeaponComponent1->CurrentWeapon->MontageData.BowAim);
		gameMode->OrderPlayerTeam(&gameMode->CharacterStateManager.ManagerOrderHoldPosition);
	}
}

void APlayerCharacter::enemyOrderHander()
{
	if (WeaponComponent1->weaponState == EWeaponState::NONE) {
		Anim->Montage_Play(WeaponComponent1->CurrentWeapon->MontageData.BowAim);
		gameMode->OrderEnemyTeam(&gameMode->CharacterStateManager.ManagerOrderEngageBattle);
	}
}

void APlayerCharacter::soundPlayer()
{
	UGameplayStatics::PlaySoundAtLocation(this, hornSound, GetActorLocation());
}
