#include "Characters/MBAIBaseCharacter.h"
#include "AI/MBStateManager.h"
#include "Datas/MBStructs.h"
#include <cmath>
#include "MBDebug.h"
#include <typeinfo>
#include "Animation/AnimInstance.h"

AMBAIBaseCharacter::AMBAIBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AMBAIBaseCharacter::InitCharacter(USkeletalMesh* InSkeletalMesh, UStaticMesh* InSpearMesh, UAnimBlueprint* InAnimBlueprint, UAnimMontage* InMontage, AIInfoData* InSelfInfo, MBStateManager* InStateManager)
{
	SkeletalMeshComponent->SetSkeletalMesh(InSkeletalMesh);
	SkeletalMeshComponent->SetAnimInstanceClass(InAnimBlueprint->GeneratedClass);

	FRotator MeshRotation = FRotator(0.f, 270.f, 0.f);
	FVector MeshLocation = FVector(0.f, 0.f, -90.f);

	SkeletalMeshComponent->SetRelativeRotation(MeshRotation);
	SkeletalMeshComponent->SetRelativeLocation(MeshLocation);

	AIInfo = InSelfInfo;
	StateManager = InStateManager;
	AIState.OrderData = &StateManager->ManagerOrderHoldPosition;
	AIState.AttitudeData = &StateManager->ManagerAttitudeIdle;
	AIState.ActionData = &StateManager->ManagerActionNone;
	AIState.MoveData = &StateManager->ManagerMoveStop;

	AIState.OrderData->InItOrder(this);

	StaticMeshSpearComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
	check(StaticMeshSpearComponent);

	StaticMeshSpearComponent->SetStaticMesh(InSpearMesh);
	StaticMeshSpearComponent->RegisterComponent();
	StaticMeshSpearComponent->AttachToComponent(SkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Spear");

	CachedAnimInstance = SkeletalMeshComponent->GetAnimInstance();
	MontageSpearAttack = InMontage;
}

void AMBAIBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CachedWorld = GetWorld();
}

void AMBAIBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (true == IsDead)
		return;

	AIState.OrderData->HandleOrder(this);
}

bool AMBAIBaseCharacter::GetIsDead()
{
	return IsDead;
}

void AMBAIBaseCharacter::SetOrder(MBOrder* InOrder)
{
	AIState.OrderData = InOrder;
	AIState.OrderData->InItOrder(AIInfo->InfoSelfData);
}

void AMBAIBaseCharacter::SetForceMoveLocation(const FVector& InForceMoveLocation)
{
	ForceMoveLocation = InForceMoveLocation;
}

void AMBAIBaseCharacter::OnHit(int InDamage)
{
	Debug::Print("AI hit", FColor::Black);

	HP -= InDamage;
	if (0 >= HP)
		Dead();
}

void AMBAIBaseCharacter::PlaySpearAttackMontage(int InSection)
{
	Debug::Print("Montage Played");
	
	CachedAnimInstance->Montage_Play(MontageSpearAttack);

	switch (InSection)
	{
	case 1:
		CachedAnimInstance->Montage_JumpToSection("Attack", MontageSpearAttack);
		break;
	case 2:
		CachedAnimInstance->Montage_JumpToSection("Defend", MontageSpearAttack);
		break;
	default:
		check(nullptr);
		break;
	}
}

void AMBAIBaseCharacter::MoveForward(const FVector& InLocation, const float InSpeed)
{
	FVector Direction = InLocation - AIInfo->InfoLocation;
	FRotator TurnRotation = AIInfo->InfoRotation;

	TurnRotation.Yaw = Direction.Rotation().Yaw;
	SetActorRotation(TurnRotation);

	AddMovementInput(Direction.GetSafeNormal(), InSpeed);
}

void AMBAIBaseCharacter::MoveControl(const FVector& InLocation, const float InSpeed)
{
	FVector Direction = InLocation - AIInfo->InfoLocation;
	FRotator TurnRotation = AIInfo->InfoRotation;

	TurnRotation.Yaw = Direction.Rotation().Yaw;
	SetActorRotation(TurnRotation);
	AddMovementInput(Direction.GetSafeNormal(), InSpeed);
}

void AMBAIBaseCharacter::MoveForceLocation(const float InSpeed)
{
	//FString Str = FString::Printf(TEXT("ForceMoveLocation = %s"), *ForceMoveLocation.ToString());
	//Debug::Print(*Str);
	MoveControl(ForceMoveLocation, InSpeed);
}

void AMBAIBaseCharacter::MoveTargetLocation(const float InSpeed)
{
	MoveControl(AIInfo->InfoTargetData->AIInfo->InfoLocation, InSpeed);
}

void AMBAIBaseCharacter::MoveSideways(const float InSpeed)
{
	FVector Direction = AIInfo->InfoTargetData->AIInfo->InfoLocation - AIInfo->InfoLocation;
	FRotator TurnRotation = AIInfo->InfoRotation;

	TurnRotation.Yaw = Direction.Rotation().Yaw;
	SetActorRotation(TurnRotation);

	float RandomAngle = FMath::RandRange(110.0f, 160.0f);
	if (50 < FMath::RandRange(1, 100))
		RandomAngle = -RandomAngle;

	FRotator Rotation(0.0f, RandomAngle, 0.0f);
	FVector RotatedDirection = Rotation.RotateVector(Direction);

	AddMovementInput(RotatedDirection.GetSafeNormal(), InSpeed);
}

void AMBAIBaseCharacter::TurnToTarget()
{
	FVector Direction = AIInfo->InfoTargetData->AIInfo->InfoLocation - AIInfo->InfoLocation;
	FRotator TurnRotation = AIInfo->InfoRotation;

	TurnRotation.Yaw = Direction.Rotation().Yaw;
	SetActorRotation(TurnRotation);
}

void AMBAIBaseCharacter::CheckTargetExist()
{
	if (nullptr == AIInfo->InfoTargetData)
		IsTargetExist = false;

	IsTargetExist = true;
}

void AMBAIBaseCharacter::CalculateDistance(const FVector& InTargetLocation)
{
	if (false == IsTargetExist)
	{
		TargetDistance = Distance::None;
		return;
	}

	CalculatedTargetDistance = std::sqrt(
		std::pow(InTargetLocation.X - AIInfo->InfoLocation.X, 2) +
		std::pow(InTargetLocation.Y - AIInfo->InfoLocation.Y, 2) +
		std::pow(InTargetLocation.Z - AIInfo->InfoLocation.Z, 2)
	);
}

void AMBAIBaseCharacter::DecideTargetDistance()
{
	if (800.f < CalculatedTargetDistance)
	{
		TargetDistance = Distance::Long;
	}
	else if (300.f < CalculatedTargetDistance)
	{
		TargetDistance = Distance::Short;
	}
	else if (200.f < CalculatedTargetDistance)
	{
		TargetDistance = Distance::Combat;
	}
	else
	{
		TargetDistance = Distance::TooClose;
	}
}

void AMBAIBaseCharacter::CheckForceLocationArrive()
{
	CalculatedTargetDistance = std::sqrt(
		std::pow(ForceMoveLocation.X - AIInfo->InfoLocation.X, 2) +
		std::pow(ForceMoveLocation.Y - AIInfo->InfoLocation.Y, 2) +
		std::pow(ForceMoveLocation.Z - AIInfo->InfoLocation.Z, 2)
	);

	if (10.f > CalculatedTargetDistance)
		IsArrivedForceLocation = true;
}

bool AMBAIBaseCharacter::IsTimerActive(FTimerHandle* InTimer)
{
	return CachedWorld->GetTimerManager().IsTimerActive(*InTimer);
}

void AMBAIBaseCharacter::ClearTimer(FTimerHandle* InTimer)
{
	CachedWorld->GetTimerManager().ClearTimer(*InTimer);
}

void AMBAIBaseCharacter::SetLeadTimer(const float InTime)
{
	CachedWorld->GetTimerManager().SetTimer(RandomLeadTimer, [this]()
		{
			bool SwitchLead = (this->AIState.AttitudeData == &this->StateManager->ManagerAttitudeAggressive) &&
				(this->AIInfo->InfoTargetData->AIState.AttitudeData == &this->StateManager->ManagerAttitudeAggressive);
			
			if (SwitchLead)
			{
				this->AIInfo->InfoTargetData->AIState.MoveData = &StateManager->ManagerMoveLead;
				this->AIState.MoveData = &StateManager->ManagerMoveChase;
				this->AIInfo->InfoTargetData->SetLeadTimer(FMath::RandRange(1.5f, 2.5f));
				this->ClearTimer(&RandomLeadTimer);
			}
		}, InTime, false);
}

void AMBAIBaseCharacter::SetActionAttackTimer(const float InAnimTime, const float InEffectStartTime, const float InEffectTime)
{
	EnableActionDelay = true;
	EnableAttackDelay = true;
	IsAttacking = true;

	CachedWorld->GetTimerManager().SetTimer(ActionDelayTimer, [this]()
		{
			this->EnableActionDelay = false;
		}, 3.f, false);
	CachedWorld->GetTimerManager().SetTimer(AttackDelayTimer, [this]()
		{
			this->EnableAttackDelay = false;
		}, 7.f, false);
	CachedWorld->GetTimerManager().SetTimer(ActionAnimTimer, [this]()
		{
			this->IsAttacking = false;
			this->AIState.ActionData = &this->StateManager->ManagerActionNone;
		}, InAnimTime, false);
	CachedWorld->GetTimerManager().SetTimer(ActionEventTimer, [this, InEffectTime]()
		{
			this->AIState.ActionData = &this->StateManager->ManagerActionStrike;
			CachedWorld->GetTimerManager().SetTimer(ActionEventTimer, [this, InEffectTime]()
				{
					this->AIState.ActionData = &this->StateManager->ManagerActionAttacking;
				}, InEffectTime, false);

		}, InEffectStartTime, false);
}

void AMBAIBaseCharacter::SetActionDefendTimer(const float InAnimTime, const float InEffectStartTime, const float InEffectTime)
{
	EnableActionDelay = true;
	IsDefending = true;

	CachedWorld->GetTimerManager().SetTimer(ActionDelayTimer, [this]()
		{
			this->EnableActionDelay = false;
		}, 3.f, false);
	CachedWorld->GetTimerManager().SetTimer(ActionAnimTimer, [this]()
		{
			this->IsDefending = false;
			this->AIState.ActionData = &this->StateManager->ManagerActionNone;
		}, InAnimTime, false);
	CachedWorld->GetTimerManager().SetTimer(ActionEventTimer, [this, InEffectTime]()
		{
			this->AIState.ActionData = &this->StateManager->ManagerActionBlock;
			CachedWorld->GetTimerManager().SetTimer(ActionEventTimer, [this, InEffectTime]()
				{
					this->AIState.ActionData = &this->StateManager->ManagerActionDefending;
				}, InEffectTime, false);

		}, InEffectStartTime, false);
}

void AMBAIBaseCharacter::SetDelayTimer(FTimerHandle* InTimer, const float InTime, bool* InValue)
{
}

UStaticMeshComponent* AMBAIBaseCharacter::GetSpearMesh()
{
	return StaticMeshSpearComponent;
}

void AMBAIBaseCharacter::Dead()
{
	Debug::Print("AI is killed", FColor::Black);
	IsDead = true;
}

//void AMBAIBaseCharacter::SpearAttackMontageStarted(UAnimMontage* InMontage)
//{
//	//FRotator NewRotation = AnimOriginTransform.GetRotation().Rotator();
//	//NewRotation.Pitch += -10.0f;
//	//FTransform NewTransform(NewRotation, AnimOriginTransform.GetLocation(), AnimOriginTransform.GetScale3D());
//
//	//FTransform BoneTransform = SkeletalMeshComponent->GetBoneTransformByName("Spear", EBoneSpaces::ComponentSpace);
//
//	//FRotator NewRotation = BoneTransform.GetRotation().Rotator();
//	//NewRotation.Pitch += PitchAdjustment;
//	//BoneTransform.SetRotation(FQuat(NewRotation));
//}
//
//void AMBAIBaseCharacter::SpearAttackMontageEnded(UAnimMontage* InMontage, bool bInterrupted)
//{
//
//}