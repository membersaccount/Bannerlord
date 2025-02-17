// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowActor.h"
#include "Components/StaticMeshComponent.h"
#include "../PlayerCharacter.h"
#include "ArrowProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
AArrowActor::AArrowActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon"));
	SetRootComponent(SceneComp);
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetCollisionProfileName(TEXT("BlockAll"));
	BoxComp->SetupAttachment(SceneComp);

	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ARROW21313"));
	ArrowComp->SetupAttachment(BoxComp);

	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow"));
	ArrowMesh->SetupAttachment(SceneComp);

	//ArrowMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//ArrowMesh->SetRelativeLocationAndRotation(FVector(-1.867894, -63.768404, -2.285775), FRotator(-37.662469, 9.346927, 54.482824));
	ArrowMesh->SetRelativeScale3D(FVector(1.3));

	arrowProjectileMovementComponent = CreateDefaultSubobject<UArrowProjectileMovementComponent>(TEXT("arrowProjectileMovementComponent"));
	arrowProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	arrowProjectileMovementComponent->InitialSpeed = 5000.0f;
	arrowProjectileMovementComponent->MaxSpeed = 5000.0f;
	arrowProjectileMovementComponent->bRotationFollowsVelocity = true;
	arrowProjectileMovementComponent->bShouldBounce = false;
	arrowProjectileMovementComponent->Bounciness = 0.3f;

}

// Called when the game starts or when spawned
void AArrowActor::BeginPlay()
{
	Super::BeginPlay();

	FVector ForwardDirection = ArrowComp->GetForwardVector();  // ArrowComponent의 정면 방향
	FRotator Rotation = ForwardDirection.Rotation();               // 그 방향에 맞는 회전값

	// ArrowComponent를 해당 방향으로 회전시킴
	ArrowComp->SetWorldRotation(Rotation);

	// 초기 속도 설정 (화살이 ArrowComponent의 방향으로 날아가게)
	FVector InitialVelocity = ForwardDirection * 5000.0f;

	// ProjectileMovementComponent의 속도 설정
	if (arrowProjectileMovementComponent)
	{
		arrowProjectileMovementComponent->Velocity = InitialVelocity;
		arrowProjectileMovementComponent->bRotationFollowsVelocity = true;  // 회전하도록 설정
	}

	// 물리 시뮬레이션 활성화
	if (UPrimitiveComponent* ArrowPhysics = Cast<UPrimitiveComponent>(this->GetRootComponent()))
	{
		ArrowPhysics->SetSimulatePhysics(true);  // 물리 시뮬레이션 활성화
		ArrowPhysics->SetEnableGravity(false);  // 발사 전 중력 비활성화
	}


}

// Called every frame
void AArrowActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AArrowActor::targetSetLocation(FVector TargetLocation)
{

}
