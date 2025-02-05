#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MBAIBaseCharacter.generated.h"

namespace Structs { namespace AI { namespace InfoData { struct AIInfoData; } } }

UCLASS()
class P1_API AMBAIBaseCharacter : public ACharacter
{
	GENERATED_BODY()

	typedef Structs::AI::InfoData::AIInfoData AIInfoData;

public:
	AMBAIBaseCharacter();
	void InitCharacter(USkeletalMesh* InSkeletalMesh, UAnimBlueprint* InAnimBlueprint, AIInfoData* InSelfInfo);
	bool IsDead;

	USkeletalMeshComponent* SkeletalMeshComponent;
	UAnimBlueprint* AnimationBlueprint;

	AIInfoData* AIInfo;

};
