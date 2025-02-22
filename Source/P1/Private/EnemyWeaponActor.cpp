// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyWeaponActor.h"

// Sets default values
AEnemyWeaponActor::AEnemyWeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	SpearMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON1"));
	SetRootComponent(SpearMesh);

	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<UStaticMesh> Spear_Weapon(TEXT("/Script/Engine.StaticMesh'/Game/LHW/Assets/Models/Mesh/Spear_Spear_LOD0.Spear_Spear_LOD0'"));
	if (Spear_Weapon.Succeeded()) {

		SpearMesh->SetStaticMesh(Spear_Weapon.Object);
	}

}

// Called when the game starts or when spawned
void AEnemyWeaponActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEnemyWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

