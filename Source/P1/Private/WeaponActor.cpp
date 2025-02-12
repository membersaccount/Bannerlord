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
#include "EMouseState.h"
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

void AWeaponActor::playAttackMontage(EWeaponState weaponState, EMouseState mouseState)
{
	if (!me) return;

	if (CachedMontages.Contains(weaponState))
	{
		UE_LOG(LogTemp, Warning, TEXT("weaponActor"));

		FWeaponMontageData MontageData = CachedMontages[weaponState];
			switch (mouseState)
			{
			case EMouseState::NONE:
				break;
			case EMouseState::UP: { if (MontageData.AttackUpMontage) me->Anim->Montage_Play(MontageData.AttackUpMontage); }
				break;
			case EMouseState::DOWN: { if (MontageData.AttackDownMontage) me->Anim->Montage_Play(MontageData.AttackDownMontage); }
				break;
			case EMouseState::RIGHT: { if (MontageData.AttackRightMontage) me->Anim->Montage_Play(MontageData.AttackRightMontage); }
				break;
			case EMouseState::LEFT: { if (MontageData.AttackLeftMontage) me->Anim->Montage_Play(MontageData.AttackLeftMontage); }
				break;
			case EMouseState::MAX:
				break;
			default:
				break;
			}
	}
}

void AWeaponActor::playGuardMontage(EWeaponState weaponState, EMouseState mouseState)
{
	if (!me) return;

	if (CachedMontages.Contains(weaponState))
	{
		UE_LOG(LogTemp, Warning, TEXT("weaponActor"));

		FWeaponMontageData MontageData = CachedMontages[weaponState];
		switch (mouseState)
		{
		case EMouseState::NONE:
			break;
		case EMouseState::UP: { if (MontageData.GuardUpMontage) me->Anim->Montage_Play(MontageData.GuardUpMontage); }
							break;
		case EMouseState::DOWN: { if (MontageData.GuardDownMontage) me->Anim->Montage_Play(MontageData.GuardDownMontage); }
							  break;
		case EMouseState::RIGHT: { if (MontageData.GuardRightMontage) me->Anim->Montage_Play(MontageData.GuardRightMontage); }
							   break;
		case EMouseState::LEFT: { if (MontageData.GuardLeftMontage) me->Anim->Montage_Play(MontageData.GuardLeftMontage); }
							  break;
		case EMouseState::MAX:
			break;
		default:
			break;
		}
	}
}

void AWeaponActor::playHitMontage()
{

}

void AWeaponActor::playDieMotage()
{

}

