#include "Characters/MBAIBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "AI/MBStateManager.h"
#include "Datas/MBStructs.h"
#include <cmath>
#include "MBDebug.h"
#include "MBSettings.h"
#include <typeinfo>

AMBAIBaseCharacter::AMBAIBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	StaticMeshSpearComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshSpearComponent->SetupAttachment(SkeletalMeshComponent);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AMBAIBaseCharacter::InitCharacter(USkeletalMesh* InSkeletalMesh, UStaticMesh* InSpearMesh, UAnimBlueprint* InAnimBlueprint, UAnimMontage* InMontageFullbody, UAnimMontage* InMontageUpperbody, AIInfoData* InSelfInfo, MBStateManager* InStateManager, bool InIsPlayerTeam)
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

	AIState.OrderData->InitOrder(this);

	StaticMeshSpearComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
	check(StaticMeshSpearComponent);

	StaticMeshSpearComponent->SetStaticMesh(InSpearMesh);
	StaticMeshSpearComponent->RegisterComponent();
	StaticMeshSpearComponent->AttachToComponent(SkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Spear");

	CachedAnimInstance = SkeletalMeshComponent->GetAnimInstance();
	MontageFullbody = InMontageFullbody;
	MontageUpperbody = InMontageUpperbody;

	SetActorEnableCollision(true);
	StaticMeshSpearComponent->SetCollisionProfileName(TEXT("enemyattack"));
	StaticMeshSpearComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	IsPlayerTeam = InIsPlayerTeam;
}
 
void AMBAIBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	CachedWorld = GetWorld();
	check(CachedWorld);
}

void AMBAIBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (true == IsDead)
		return;

	AIState.OrderData->HandleOrder(this);

#ifdef DebugMode
	ShowAIState();
#endif // DebugMode
}

bool AMBAIBaseCharacter::GetIsDead()
{
	return IsDead;
}

void AMBAIBaseCharacter::SetOrder(MBOrder* InOrder)
{
	AIState.OrderData = InOrder;
	AIState.OrderData->InitOrder(AIInfo->InfoSelfData);
}

void AMBAIBaseCharacter::SetForceMoveLocation(const FVector& InForceMoveLocation)
{
	ForceMoveLocation = InForceMoveLocation;
}

int AMBAIBaseCharacter::OnHit(int InDamage, bool IsPlayer)
{
#ifdef DebugMode
	//Debug::Print("AI hit", FColor::Black);
#endif // DebugMode

	if (IsPlayer && IsPlayerTeam)
		return 3;

	if (true == IsDead)
		return 0;

	HP -= InDamage;
	if (0 >= HP)
	{
		Dead();
		return 1;
	}

	PlayMontageHit();
	AIState.MoveData->ExecuteMove(this);

	if (AIState.ActionData == &StateManager->ManagerActionBlock)
	{
		return 2;
	}
	return 0;
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

void AMBAIBaseCharacter::MoveControl(const FVector& InLocation, const FVector& InRotationLocation, const float InSpeed)
{
	FVector LookDirection = InRotationLocation - AIInfo->InfoLocation;
	FRotator TurnRotation = AIInfo->InfoRotation;

	TurnRotation.Yaw = LookDirection.Rotation().Yaw;
	SetActorRotation(TurnRotation);

	FVector MoveDirection = InLocation - AIInfo->InfoLocation;
	AddMovementInput(MoveDirection.GetSafeNormal(), InSpeed);
}

void AMBAIBaseCharacter::MoveForceLocation(const float InSpeed)
{
	IsMovingbackwards = false;
	MoveControl(ForceMoveLocation, InSpeed);
}

void AMBAIBaseCharacter::MoveTargetLocation(const float InSpeed)
{
#ifdef DebugModeBugFix_Move
	//Debug::NullCheck(AIInfo->InfoTargetData->AIInfo, "AIInfo->InfoTargetData->AIInfo");
#endif // DebugModeBugFix_Move

	if (0 > InSpeed)
	{
		IsMovingbackwards = true;

		//int Tendency = FMath::RandRange(1, 100);
		//CalculateTeamCenterDistance();

		//if (10000.f < CalculatedTeamCenterDistance)
		//{
		//	FVector Direction = AIInfo->TeamCenter - AIInfo->InfoLocation;
		//	FVector OppositeDirection = FVector(-Direction.X, -Direction.Y, Direction.Z);
		//	FVector OppositeLocation = AIInfo->InfoLocation + OppositeDirection;
		//	return;
		//}

		//if (20 > Tendency)
		//{
		//	FVector Direction = AIInfo->TeamCenter - AIInfo->InfoLocation;
		//	FVector OppositeDirection = FVector(-Direction.X, -Direction.Y, Direction.Z);
		//	FVector OppositeLocation = AIInfo->InfoLocation + OppositeDirection;

		//	MoveControl(OppositeLocation, AIInfo->InfoTargetData->AIInfo->InfoLocation, InSpeed);
		//	return;
		//}

		MoveControl(AIInfo->InfoTargetData->AIInfo->InfoLocation, InSpeed);
	}
	else
	{
		IsMovingbackwards = false;
	}

	MoveControl(AIInfo->InfoTargetData->AIInfo->InfoLocation, InSpeed);
}

void AMBAIBaseCharacter::MoveSideways(const float InSpeed)
{
	IsMovingbackwards = true;

	FVector Direction = AIInfo->InfoTargetData->AIInfo->InfoLocation - AIInfo->InfoLocation;
	FRotator TurnRotation = AIInfo->InfoRotation;

	TurnRotation.Yaw = Direction.Rotation().Yaw;
	SetActorRotation(TurnRotation);

	int Tendency = FMath::RandRange(1, 100);
	CalculateTeamCenterDistance();
	
	//if (10000.f < CalculatedTeamCenterDistance)
	//{
	//	FVector TeamCenterDirection = AIInfo->TeamCenter - AIInfo->InfoLocation;
	//	FVector OppositeDirection = FVector(-TeamCenterDirection.X, -TeamCenterDirection.Y, TeamCenterDirection.Z);

	//	AddMovementInput(OppositeDirection.GetSafeNormal(), InSpeed);
	//	return;
	//}

	//if (30 > Tendency)
	//{
	//	FVector TeamCenterDirection = AIInfo->TeamCenter - AIInfo->InfoLocation;
	//	FVector OppositeDirection = FVector(-TeamCenterDirection.X, -TeamCenterDirection.Y, TeamCenterDirection.Z);

	//	AddMovementInput(OppositeDirection.GetSafeNormal(), InSpeed);
	//	return;
	//}

	float RandomAngle = FMath::RandRange(110.0f, 140.0f);
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

void AMBAIBaseCharacter::TurnToFront()
{
	FRotator Front = FRotator(0.f, 180.f, 0.f);
	SetActorRotation(Front);
}

void AMBAIBaseCharacter::CheckTargetExist()
{
	if (true == AIInfo->InfoTargetData->AIInfo->InfoSelfData->IsDead)
	{
		TargetDistance = Distance::None;
		IsTargetExist = false;
	}

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

void AMBAIBaseCharacter::CalculateTeamCenterDistance()
{
	CalculatedTeamCenterDistance =
		std::pow(AIInfo->TeamCenter.X - AIInfo->InfoLocation.X, 2) +
		std::pow(AIInfo->TeamCenter.Y - AIInfo->InfoLocation.Y, 2) +
		std::pow(AIInfo->TeamCenter.Z - AIInfo->InfoLocation.Z, 2);
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
		std::pow(ForceMoveLocation.Y - AIInfo->InfoLocation.Y, 2)
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

void AMBAIBaseCharacter::SetActionAttackTimer(const float InAnimTime, const float InEffectTime)
{
#ifdef DebugModeBugFix_Action
	if (IsPlayerTeam)
		Debug::Print("--- Attack TimerSet ---");
#endif // DebugModeBugFix_Action

	EnableActionDelay = true;
	EnableAttackDelay = true;
	IsAttacking = true;

#ifdef DebugModeBugFix_Action
	if (IsPlayerTeam)
	{
		Debug::Print("EnableActionDelay = true");
		Debug::Print("EnableAttackDelay = true");
		Debug::Print("IsAttacking = true");
	}
#endif // DebugModeBugFix_Action

	CachedWorld->GetTimerManager().SetTimer(ActionAnimTimer, [this]()
		{
			this->IsAttacking = false;
			this->AIState.ActionData = &this->StateManager->ManagerActionNone;
#ifdef DebugModeBugFix_Action
			if (IsPlayerTeam)
				Debug::Print("IsAttacking = false");
#endif // DebugModeBugFix_Action
		}, InAnimTime, false);

	CachedWorld->GetTimerManager().SetTimer(ActionDelayTimer, [this]()
		{
			this->EnableActionDelay = false;
		}, FMath::RandRange(3.5f, 5.5f), false);

	CachedWorld->GetTimerManager().SetTimer(AttackDelayTimer, [this]()
		{
			this->EnableAttackDelay = false;
			this->AIState.ActionData = &this->StateManager->ManagerActionNone;
#ifdef DebugModeBugFix_Action
			if (IsPlayerTeam)
				Debug::Print("EnableActionDelay = false");
#endif // DebugModeBugFix_Action
		}, 5.f, false);

	CachedWorld->GetTimerManager().SetTimer(ActionEventTimer, [this]()
		{
			this->AIState.ActionData = &this->StateManager->ManagerActionStrike;
		}, InEffectTime, false);
}

void AMBAIBaseCharacter::SetActionDefendTimer(const float InAnimTime, const float InEffectStartTime, const float InEffectTime)
{
#ifdef DebugModeBugFix_Action
	if (IsPlayerTeam)
		Debug::Print("--- Defend TimerSet ---");
#endif // DebugModeBugFix_Action

	EnableActionDelay = true;
	IsDefending = true;

#ifdef DebugModeBugFix_Action
	if (IsPlayerTeam)
	{
		Debug::Print("EnableActionDelay = true");
		Debug::Print("IsDefending = true");
	}
#endif // DebugModeBugFix_Action

	CachedWorld->GetTimerManager().SetTimer(ActionAnimTimer, [this]()
		{
			this->IsDefending = false;
			this->AIState.ActionData = &this->StateManager->ManagerActionNone;
#ifdef DebugModeBugFix_Action
			if (IsPlayerTeam)
				Debug::Print("IsDefending = false");
#endif // DebugModeBugFix_Action
		}, InAnimTime, false);

	CachedWorld->GetTimerManager().SetTimer(ActionDelayTimer, [this]()
		{
			this->EnableActionDelay = false;
			this->AIState.ActionData = &this->StateManager->ManagerActionNone;
#ifdef DebugModeBugFix_Action
			if (IsPlayerTeam)
				Debug::Print("EnableActionDelay = false");
#endif // DebugModeBugFix_Action
		}, FMath::RandRange(3.5f, 5.5f), false);

	CachedWorld->GetTimerManager().SetTimer(ActionEventTimer, [this, InEffectTime]()
		{
			this->AIState.ActionData = &this->StateManager->ManagerActionBlock;
			CachedWorld->GetTimerManager().SetTimer(ActionEventTimer, [this, InEffectTime]()
				{
					this->AIState.ActionData = &this->StateManager->ManagerActionDefending;
				}, InEffectTime, false);
		}, InEffectTime, false);
}

void AMBAIBaseCharacter::SetDelayTimer(FTimerHandle* InTimer, const float InTime, bool* InValue)
{
}

void AMBAIBaseCharacter::PlayMontageAttack(int InType)
{
	CachedAnimInstance->Montage_Play(MontageUpperbody);

	switch (InType)
	{
	case 1:
		CachedAnimInstance->Montage_JumpToSection("AttackDown", MontageUpperbody);
		break;
	case 2:
		CachedAnimInstance->Montage_JumpToSection("AttackUp", MontageUpperbody);
		break;
	case 3:
		CachedAnimInstance->Montage_JumpToSection("AttackRight", MontageUpperbody);
		break;
	default:
		check(false);
		break;
	}
}

void AMBAIBaseCharacter::PlayMontageHit()
{
	CachedAnimInstance->Montage_Play(MontageUpperbody);
	CachedAnimInstance->Montage_JumpToSection("Hit", MontageUpperbody);
}

void AMBAIBaseCharacter::PlayMontageDefend()
{
	CachedAnimInstance->Montage_Play(MontageUpperbody);
	CachedAnimInstance->Montage_JumpToSection("Defend", MontageUpperbody);
}

void AMBAIBaseCharacter::PlayMontageDead()
{
	CachedAnimInstance->Montage_Play(MontageFullbody);
	CachedAnimInstance->Montage_JumpToSection("Dead", MontageFullbody);
}

void AMBAIBaseCharacter::Dead()
{
	IsDead = true;
	ClearTimer(&ActionAnimTimer);
	ClearTimer(&ActionEventTimer);
	ClearTimer(&ActionDelayTimer);
	ClearTimer(&AttackDelayTimer);
	ClearTimer(&RandomLeadTimer);
	ClearTimer(&DebugTimer);

	this->GetCapsuleComponent();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PlayMontageDead();
	CachedWorld->GetTimerManager().SetTimer(DebugTimer, [this]()
		{
			this->Destroy();
		}, 3.f, false);
}

#ifdef DebugMode
void AMBAIBaseCharacter::ShowAIState()
{
	switch (AIAttitude)
	{
	case Attitude::Idle:
		DrawDebug::DrawSphere(CachedWorld, AIInfo->InfoLocation + FVector(0.f, 0.f, 100.f));
		break;
	case Attitude::Aggressive:
		DrawDebug::DrawSphere(CachedWorld, AIInfo->InfoLocation + FVector(0.f, 0.f, 100.f), FColor::Red);
		break;
	case Attitude::Defensive:
		DrawDebug::DrawSphere(CachedWorld, AIInfo->InfoLocation + FVector(0.f, 0.f, 100.f), FColor::Yellow);
		break;
	}

	switch (AIAction)
	{
	case Action::None:
		break;
	case Action::Attacking:
		DrawDebug::DrawString(CachedWorld, AIInfo->InfoLocation + FVector(0.f, 0.f, 100.f), "Attacking", FColor::Red);
		break;
	case Action::Strike:
		DrawDebug::DrawString(CachedWorld, AIInfo->InfoLocation + FVector(0.f, 0.f, 100.f), "Strike", FColor::Yellow, 3.f);
		break;
	case Action::Defending:
		break;
	case Action::Block:
		DrawDebug::DrawString(CachedWorld, AIInfo->InfoLocation + FVector(0.f, 0.f, 100.f), "Blocking", FColor::Cyan);
		break;
	}

	switch (AIMove)
	{
	case Move::Charge:
		break;
	case Move::Chase:
		break;
	case Move::Lead:
		break;
	case Move::Avoid:
		break;
	default:
		break;
	}
}
#endif // DebugMode
