// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "WeaponActor.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	weaponState = EWeaponState::NONE;
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	switch (weaponState)
	{
	case EWeaponState::NONE:
		break;
	case EWeaponState::SPEAR:
		break;
	case EWeaponState::SWORD:
		break;
	case EWeaponState::BOW:
		break;
	default:
		break;
	}
}

void UWeaponComponent::changeWeaponState(EWeaponState eweapon)
{
	//캐릭터에서 상태값 가져오기
	if (weaponState == eweapon)return;
	weaponState = eweapon;
	//기본 애니메이션 변경
	changeWeaponAnamation();
}

void UWeaponComponent::changeWeaponAnamation()
{
	FString log = UEnum::GetValueAsString(weaponState);
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, log);
	CurrentWeapon->playChangeMontage(weaponState);
}

void UWeaponComponent::attackUpHandler()
{

}

void UWeaponComponent::attackDownHandler()
{

}

void UWeaponComponent::attackRightHandler()
{

}

void UWeaponComponent::attackLeftHandler()
{

}

void UWeaponComponent::hitActionHandler()
{

}

void UWeaponComponent::dieActionHandler()
{

}

