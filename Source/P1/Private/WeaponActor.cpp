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
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Components/SceneComponent.h"
#include "BowAnimInstance.h"
#include "PlayerAnimInstance.h"
#include "Characters/MBAISpearman.h"
#include "Components/Image.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Classes/NiagaraSystem.h"
#include "UObject/ConstructorHelpers.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"
// Sets default values
AWeaponActor::AWeaponActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> TempNiagaraSystem(
		TEXT("/Script/Niagara.NiagaraSystem'/Game/LHW/Assets/Blood_VFX_Pack/Particles/Systems/P_BigSplash_Hit.P_BigSplash_Hit'"));

	if (TempNiagaraSystem.Succeeded())
	{
		BloodSplatterNiagara = TempNiagaraSystem.Object;
	}
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon"));
	SetRootComponent(SceneComp);
	loadWeapon();


}
// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();

	SpearMesh->OnComponentBeginOverlap.AddDynamic(this, &AWeaponActor::overlapEvent);
	SpearMesh->OnComponentEndOverlap.AddDynamic(this, &AWeaponActor::OnEndOverlap);

	SwordMesh->OnComponentBeginOverlap.AddDynamic(this, &AWeaponActor::overlapEvent);
	SwordMesh->OnComponentEndOverlap.AddDynamic(this, &AWeaponActor::OnEndOverlap);

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
			BowMesh->SetRelativeLocationAndRotation(CachedMontages[EWeaponState::BOW].weaponPosition, CachedMontages[EWeaponState::BOW].weaponRotation);
			SwordMesh->SetRelativeLocationAndRotation(CachedMontages[EWeaponState::SWORD].weaponPosition, CachedMontages[EWeaponState::SWORD].weaponRotation);
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
	weaponStateIn = weaponState;
	me->isAttack = false;
	if (CachedMontages.Contains(weaponState))
	{
		MontageData = CachedMontages[weaponState];

		if (MontageData.ChangeWeaponMontage)
		{
			// 무기 변경 애니메이션 실행
			//selectWeapon(weaponState);

			me->Anim->Montage_Play(MontageData.ChangeWeaponMontage);
		}
	}
}

void AWeaponActor::playAttackMontage(EWeaponState weaponState, EMouseState mouseState)
{
	if (!me) return;


	if (CachedMontages.Contains(weaponState))
	{
		MontageData = CachedMontages[weaponState];
		if (weaponState == EWeaponState::BOW) { BowMesh->GetAnimInstance()->Montage_Play(MontageData.BowAim); }

		switch (mouseState)
		{
		case EMouseState::NONE:
			break;
		case EMouseState::UP: { if (MontageData.AttackUpMontage) me->Anim->Montage_Play(MontageData.AttackUpMontage, 0.7f); }
							break;
		case EMouseState::DOWN: { if (MontageData.AttackDownMontage) me->Anim->Montage_Play(MontageData.AttackDownMontage, 0.7f); }
							  break;
		case EMouseState::RIGHT: { if (MontageData.AttackRightMontage) me->Anim->Montage_Play(MontageData.AttackRightMontage, 0.7f); }
							   break;
		case EMouseState::LEFT: { if (MontageData.AttackLeftMontage) me->Anim->Montage_Play(MontageData.AttackLeftMontage, 0.7f); }
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

		MontageData = CachedMontages[weaponState];

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

void AWeaponActor::loadWeapon()
{

	SpearMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON1"));
	BowMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON2"));
	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON3"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Spear_Weapon(TEXT("/Script/Engine.StaticMesh'/Game/LHW/Assets/Models/Mesh/Spear_Spear_LOD0.Spear_Spear_LOD0'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Bow_Weapon(TEXT("/ Script / Engine.SkeletalMesh'/Game/LHW/ArcherAnimsetPro/Meshes/Bow/SK_Bow.SK_Bow'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Sword_Weapon(TEXT("/Script/Engine.StaticMesh'/Game/LHW/Assets/sword/Weapons/Sword/SM_Sword.SM_Sword'"));
	if (Spear_Weapon.Succeeded() && Bow_Weapon.Succeeded() && Sword_Weapon.Succeeded()) {

		SpearMesh->SetStaticMesh(Spear_Weapon.Object);
		BowMesh->SetSkeletalMesh(Bow_Weapon.Object);
		SwordMesh->SetStaticMesh(Sword_Weapon.Object);

		SpearMesh->SetupAttachment(SceneComp);
		BowMesh->SetupAttachment(SceneComp);
		SwordMesh->SetupAttachment(SceneComp);

		SpearMesh->SetVisibility(false);
		BowMesh->SetVisibility(false);
		SwordMesh->SetVisibility(false);


		//SpearMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		BowMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//SwordMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SpearMesh->SetCollisionProfileName(TEXT("spear"));
		SwordMesh->SetCollisionProfileName(TEXT("spear"));
		this->SetActorEnableCollision(true);

		BowMesh->SetAnimInstanceClass(UBowAnimInstance::StaticClass());
	}


}

void AWeaponActor::selectWeapon()
{
	switch (weaponStateIn)
	{
	case EWeaponState::NONE: {
		SpearMesh->SetVisibility(false);
		BowMesh->SetVisibility(false);
		SwordMesh->SetVisibility(false);
		me->BowMesh->SetVisibility(true);
		me->SpearMesh->SetVisibility(true);
	}
						   break;
	case EWeaponState::SPEAR: {
		SpearMesh->SetVisibility(true);
		BowMesh->SetVisibility(false);
		SwordMesh->SetVisibility(false);
		me->SpearMesh->SetVisibility(false);
		me->BowMesh->SetVisibility(true);

	}
							break;
	case EWeaponState::SWORD: {
		SpearMesh->SetVisibility(false);
		BowMesh->SetVisibility(false);
		SwordMesh->SetVisibility(true);
		me->BowMesh->SetVisibility(true);
		me->SpearMesh->SetVisibility(true);

	}
							break;
	case EWeaponState::BOW: {
		SpearMesh->SetVisibility(false);
		BowMesh->SetVisibility(true);
		SwordMesh->SetVisibility(false);
		me->BowMesh->SetVisibility(false);
		me->SpearMesh->SetVisibility(true);
	}
						  break;
	default:
		break;
	}
}

void AWeaponActor::showCrossHair(bool isHit)
{
	if (isHit)
		me->WeaponComponent1->CrossHair->SetOpacity(1);
	else
		me->WeaponComponent1->CrossHair->SetOpacity(0);


}

void AWeaponActor::overlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherComp) return;
	UPlayerAnimInstance* a = Cast<UPlayerAnimInstance>(me->Anim);
	AMBAISpearman* Enemy = Cast<AMBAISpearman>(OtherActor);
	if (Enemy) {
		me->isAttack = false;

		Enemy->OnHit(MontageData.damage);
		showCrossHair(true);
		//auto currentMontage = me->Anim->GetCurrentActiveMontage();
		//if (me->Anim->Montage_IsPlaying(currentMontage))
		//{
		//	me->Anim->Montage_SetPlayRate(currentMontage, 0);

		//	// 현재 재생 위치 얻기

		//	FTimerHandle visibleTime1;
		//	FTimerDelegate TimerLambda = FTimerDelegate::CreateLambda([this]() {
		//		auto currentMontage = me->Anim->GetCurrentActiveMontage();

		//		me->Anim->Montage_SetPlayRate(currentMontage, 0.7f); });
		//	GetWorld()->GetTimerManager().SetTimer(visibleTime1, TimerLambda, 0.5f, false);


		//}
		FTimerHandle visibleTime;
		FTimerDelegate TimerLambda = FTimerDelegate::CreateLambda([this]() { showCrossHair(false); });
		GetWorld()->GetTimerManager().SetTimer(visibleTime, TimerLambda, 1.0f, false);
		if (BloodSplatterNiagara)
		{
			FVector HitLocation = SweepResult.ImpactPoint;
			auto BloodComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodSplatterNiagara, OtherActor->GetActorLocation());
		}

	}

	if (me->Anim && Enemy == nullptr)
	{
		me->isAttack = false;

		//UE_LOG(LogTemp, Warning, TEXT("Weapon Overlap"));
		// 현재 애니메이션이 재생 중인지 확인
		if (me->Anim->Montage_IsPlaying(me->Anim->GetCurrentActiveMontage()))
		{
			// 애니메이션을 일시 정지 (현재 위치에서 멈추기)
			me->Anim->Montage_Pause(me->Anim->GetCurrentActiveMontage());

			// 현재 재생 위치 얻기
			float CurrentPosition = me->Anim->Montage_GetPosition(me->Anim->GetCurrentActiveMontage());

			// 역방향으로 애니메이션 재생 (현재 위치에서 역재생)
			me->Anim->Montage_Play(me->Anim->GetCurrentActiveMontage(), -1.0f);
			me->Anim->Montage_SetPosition(me->Anim->GetCurrentActiveMontage(), CurrentPosition);
		}
	}
}

void AWeaponActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	SpearMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SwordMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

