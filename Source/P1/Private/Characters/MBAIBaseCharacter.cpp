#include "Characters/MBAIBaseCharacter.h"
#include "AI/MBStateManager.h"
#include "Datas/MBStructs.h"
#include <cmath>
#include "MBDebug.h"
#include <typeinfo>

AMBAIBaseCharacter::AMBAIBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AMBAIBaseCharacter::InitCharacter(USkeletalMesh* InSkeletalMesh, UAnimBlueprint* InAnimBlueprint, AIInfoData* InSelfInfo, MBStateManager* InStateManager)
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

	
	if (CurrentTime > FMath::RandRange(5.f, 8.f))
	{
		CurrentTime = 0.f;
		IsAttacking = true;
		CachedWorld->GetTimerManager().SetTimer(DebugTimer, [this]()
			{
				this->IsAttacking = false;
			}, 2.3f, false);
	}
	else
	{
		CurrentTime += DeltaTime;
	}
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
				this->AIInfo->InfoTargetData->SetLeadTimer(FMath::RandRange(3.f, 8.f));
				this->ClearTimer(&RandomLeadTimer);
			}
		}, InTime, false);
}

void AMBAIBaseCharacter::SetTimer(FTimerHandle* const InTimer, const float InTime)
{
}
