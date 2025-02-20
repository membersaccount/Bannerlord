#include "Characters/MBAISpearman.h"
#include "AI/MBStateManager.h"
#include "MBDebug.h"

AMBAISpearman::AMBAISpearman()
{

}

void AMBAISpearman::BeginPlay()
{
	Super::BeginPlay();

}

void AMBAISpearman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMBAISpearman::InitCharacter(USkeletalMesh* InSkeletalMesh, UStaticMesh* InSpearMesh, UAnimBlueprint* InAnimBlueprint, UAnimMontage* InMontage, AIInfoData* InSelfInfo, MBStateManager* InStateManager)
{
	Super::InitCharacter(InSkeletalMesh, InSpearMesh, InAnimBlueprint, InMontage, InSelfInfo, InStateManager);

	UStaticMeshComponent* TempMesh = GetSpearMesh();

	GetSpearMesh()->SetGenerateOverlapEvents(true);
	GetSpearMesh()->SetCollisionProfileName(TEXT("enemyattack"));
	GetSpearMesh()->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
}

void AMBAISpearman::InitSpear()
{
}

void AMBAISpearman::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Debug::Print("Spear Hit");

	if (false == IsTargetExist)
	{
		return;
	}

	if (Distance::Combat == TargetDistance || Distance::TooClose == TargetDistance)
	{
		if (&StateManager->ManagerActionDefending != AIInfo->InfoTargetData->AIState.ActionData)
		{
			return;
		}
	}

	//AIInfo->InfoTargetData->OnHit(Damage);
	AIInfo->InfoTargetData->OnHit(0);
	AIState.ActionData = &StateManager->ManagerActionAttacking;
	
	GetSpearMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
