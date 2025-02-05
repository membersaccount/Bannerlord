#include "Characters/MBAIBaseCharacter.h"
#include "Datas/MBStructs.h"

AMBAIBaseCharacter::AMBAIBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AMBAIBaseCharacter::InitCharacter(USkeletalMesh* InSkeletalMesh, UAnimBlueprint* InAnimBlueprint, AIInfoData* InSelfInfo)
{
	SkeletalMeshComponent->SetSkeletalMesh(InSkeletalMesh);
	SkeletalMeshComponent->SetAnimInstanceClass(InAnimBlueprint->GeneratedClass);

	FRotator MeshRotation = FRotator(0.f, 270.f, 0.f);
	FVector MeshLocation = FVector(0.f, 0.f, -90.f);

	SkeletalMeshComponent->SetRelativeRotation(MeshRotation);
	SkeletalMeshComponent->SetRelativeLocation(MeshLocation);

	AIInfo = InSelfInfo;
}

void AMBAIBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CachedWorld = GetWorld();
}

void AMBAIBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveForward(AIInfo->InfoTargetData->AIInfo->InfoLocation, 0.5f);
}

bool AMBAIBaseCharacter::GetIsDead()
{
	return IsDead;
}

void AMBAIBaseCharacter::MoveForward(const FVector& InLocation, const float InSpeed)
{
	FVector Direction = InLocation - AIInfo->InfoLocation;
	FRotator TurnRotation = AIInfo->InfoRotation;

	TurnRotation.Yaw = Direction.Rotation().Yaw;
	SetActorRotation(TurnRotation);

	AddMovementInput(Direction.GetSafeNormal(), InSpeed);
}
