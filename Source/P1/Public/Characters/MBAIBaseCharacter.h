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
	void InitCharacter(USkeletalMesh* InSkeletalMesh, UStaticMesh* InSpearMesh, UAnimBlueprint* InAnimBlueprint, UAnimMontage* InMontageFullbody, UAnimMontage* InMontageUpperbody, AIInfoData* InSelfInfo, MBStateManager* InStateManager);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public: // Called Outside
	bool GetIsDead();
	void SetOrder(MBOrder* InOrder);
	void SetForceMoveLocation(const FVector& InForceMoveLocation);

	int OnHit(int InDamage);

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
	void CheckForceLocationArrive();

public: // Timer
	bool IsTimerActive(FTimerHandle* InTimer);
	void ClearTimer(FTimerHandle* InTimer);
	void SetLeadTimer(const float InTime);
	void SetActionAttackTimer(const float InAnimTime, const float InEffectStartTime, const float InEffectTime);
	void SetActionDefendTimer(const float InAnimTime, const float InEffectStartTime, const float InEffectTime);

	void SetDelayTimer(FTimerHandle* InTimer, const float InTime, bool* InValue);

public: // Animation
	void PlayMontageAttack(int InType);
	void PlayMontageHit();
	void PlayMontageDefend();
	void PlayMontageDead();

private: // Data Handle
	void Dead();

public: // Default Data
	AIInfoData* AIInfo;
	State AIState;
	MBStateManager* StateManager;

	int HP = 10000;
	int Damage = 25;
	float CalculatedTargetDistance = 0.f;

public: // AI
	Distance TargetDistance = Distance::None;
	Direction AttackDirection = Direction::None;

	bool IsTargetExist = true;
	bool IsNearForceLocation = false;
	bool IsArrivedForceLocation = false;

	bool EnableActionDelay = false;
	bool EnableAttackDelay = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsIdle = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsAttacking = false;	// 0.65 ~ 1.3, 2.3
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsDefending = false;	// 0.2 ~ 1.1 -> 1.5
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsMovingbackwards = false;

public: // Timer
	FTimerHandle ActionAnimTimer;
	FTimerHandle ActionEventTimer;
	FTimerHandle ActionDelayTimer;
	FTimerHandle AttackDelayTimer;
	FTimerHandle RandomLeadTimer;
	FTimerHandle SwitchTargetTimer;
	FTimerHandle FormationTimer;
	FTimerHandle HitTimer;
	FTimerHandle DebugTimer;

protected:
	bool IsDead;

private:
	FVector ForceMoveLocation;

public: // Default Data
	USkeletalMeshComponent* SkeletalMeshComponent;
	UStaticMeshComponent* StaticMeshSpearComponent;
	UAnimBlueprint* AnimationBlueprint;
	UAnimMontage* MontageFullbody;
	UAnimMontage* MontageUpperbody;

protected: // Cached Data
	UWorld* CachedWorld;
	UAnimInstance* CachedAnimInstance;

private: // Debug
	float CurrentTime = 0.f;
};
