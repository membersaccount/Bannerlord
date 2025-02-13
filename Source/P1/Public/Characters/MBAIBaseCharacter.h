#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Datas/MBStructs.h"
#include "Datas/MBEnums.h"
#include "MBAIBaseCharacter.generated.h"

class MBStateManager;

UCLASS()
class P1_API AMBAIBaseCharacter : public ACharacter
{
	GENERATED_BODY()

	typedef Structs::AI::AIInfoData AIInfoData;
	typedef Structs::AI::State State;
	typedef Structs::AI::StateType StateType;
	typedef Enums::AI::States::Distance Distance;
	typedef Enums::AI::States::Direction Direction;

public: // Init
	AMBAIBaseCharacter();
	void InitCharacter(USkeletalMesh* InSkeletalMesh, UAnimBlueprint* InAnimBlueprint, AIInfoData* InSelfInfo, MBStateManager* InStateManager);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public: // Called Outside
	bool GetIsDead();
	void SetOrder(MBOrder* InOrder);
	void SetForceMoveLocation(const FVector& InForceMoveLocation);

public: // Move
	void MoveForward(const FVector& InLocation, const float InSpeed);
	void MoveControl(const FVector& InLocation, const float InSpeed);
	void MoveForceLocation(const float InSpeed);
	void MoveTargetLocation(const float InSpeed);
	void MoveSideways(const float InSpeed);
	void TurnToTarget();

public: // Target
	void CheckTargetExist();
	void CalculateDistance(const FVector& InTargetLocation);
	void DecideTargetDistance();

public: // Timer
	bool IsTimerActive(FTimerHandle* InTimer);
	void ClearTimer(FTimerHandle* InTimer);
	void SetLeadTimer(const float InTime);
	void SetTimer(FTimerHandle* const InTimer, const float InTime);

public: // Default Data
	AIInfoData* AIInfo;
	State AIState;
	MBStateManager* StateManager;

	int HP = 100;
	float CalculatedTargetDistance = 0.f;

public: // AI
	Distance TargetDistance = Distance::None;
	Direction AttackDirection = Direction::None;

	bool IsTargetExist = true;
	bool IsNearForceLocation = false;
	bool IsArrivedForceLocation = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsAttacking = false;

public: // Timer
	FTimerHandle ActionTimer;
	FTimerHandle AttackRateTimer;
	FTimerHandle RandomLeadTimer;
	FTimerHandle SwitchTargetTimer;
	FTimerHandle FormationTimer;
	FTimerHandle DebugTimer;

protected:
	bool IsDead;
	bool CanAttack;

private:
	FVector ForceMoveLocation;

private: // Default Data
	USkeletalMeshComponent* SkeletalMeshComponent;
	UAnimBlueprint* AnimationBlueprint;

protected: // Cached Data
	UWorld* CachedWorld;

private: // Debug
	float CurrentTime = 0.f;
};
