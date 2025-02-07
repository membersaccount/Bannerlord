#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MBAIBaseCharacter.generated.h"

namespace Structs { namespace AI { 
	struct AIInfoData;
	struct State;
} } 

UCLASS()
class P1_API AMBAIBaseCharacter : public ACharacter
{
	GENERATED_BODY()

	typedef Structs::AI::AIInfoData AIInfoData;
	typedef Structs::AI::State State;

public: // Init
	AMBAIBaseCharacter();
	void InitCharacter(USkeletalMesh* InSkeletalMesh, UAnimBlueprint* InAnimBlueprint, AIInfoData* InSelfInfo);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public: // Get
	bool GetIsDead();

public:
	void MoveForward(const FVector& InLocation, const float InSpeed);

protected:
	AIInfoData* AIInfo;
	bool IsDead;
	bool CanAttack;





private: // Default Data
	USkeletalMeshComponent* SkeletalMeshComponent;
	UAnimBlueprint* AnimationBlueprint;

protected: // Cached Data
	UWorld* CachedWorld;
};
