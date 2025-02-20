#pragma once

#include "CoreMinimal.h"
#include "Characters/MBAIBaseCharacter.h"
#include "Datas/MBStructs.h"
#include "MBAISpearman.generated.h"

UCLASS()
class P1_API AMBAISpearman : public AMBAIBaseCharacter
{
	GENERATED_BODY()

	typedef Structs::AI::AIInfoData AIInfoData;

public:
	AMBAISpearman();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void InitCharacter(USkeletalMesh* InSkeletalMesh, UStaticMesh* InSpearMesh, UAnimBlueprint* InAnimBlueprint, UAnimMontage* InMontage, AIInfoData* InSelfInfo, MBStateManager* InStateManager) override;
	void InitSpear();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
