// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "EWeaponState.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "WeaponMontageData.h"
#include "WeaponMontageDataAsset.h"
#include "../PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AWeaponActor::AWeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Spear_Weapon(TEXT("/Script/Engine.StaticMesh'/Game/LHW/Assets/Models/Mesh/Spear_Spear_LOD0.Spear_Spear_LOD0'"));

	if (Spear_Weapon.Succeeded()) {
		Weapon->SetStaticMesh(Spear_Weapon.Object);
	}
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);


}
// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	//AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass());
	//if (actor) {
	auto MyOwner = GetOwner();
	if (MyOwner)
		UE_LOG(LogTemp, Warning, TEXT("My Owner Name : %s"), *MyOwner->GetName());

	me = Cast<APlayerCharacter>(MyOwner);
	//}
	if (MontageDataTable)
	{
		UWeaponMontageDataAsset* MontageDataAsset = Cast<UWeaponMontageDataAsset>(MontageDataTable->GetDefaultObject());
		if (MontageDataAsset)
		{
			CachedMontages = MontageDataAsset->WeaponMontageMap;
		}
	}




}

// Called every frame
void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AWeaponActor::playChangeMontage(EWeaponState weaponState)
{
	if (!me) return;

	if (CachedMontages.Contains(weaponState))
	{
		FWeaponMontageData MontageData = CachedMontages[weaponState];

		if (MontageData.ChangeWeaponMontage)
		{
			// 무기 변경 애니메이션 실행
			//me->PlayAnimMontage(MontageData.ChangeWeaponMontage);
			me->Anim->Montage_Play(MontageData.ChangeWeaponMontage);
		}
	}
}

void AWeaponActor::playAttackMontage()
{

}

void AWeaponActor::playHitMontage()
{

}

void AWeaponActor::playDieMotage()
{

}

