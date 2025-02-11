// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "WeaponActor.h"
#include "EMouseState.h"

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
	updateMouseDirection();
	FString log = UEnum::GetValueAsString(EmouseDirection);
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, log);

}

void UWeaponComponent::changeWeaponState(EWeaponState eweapon)
{
	//ĳ���Ϳ��� ���°� ��������
	if (weaponState == eweapon)return;
	weaponState = eweapon;
	//�⺻ �ִϸ��̼� ����
	changeWeaponAnamation();
}

void UWeaponComponent::changeWeaponAnamation()
{
	FString log = UEnum::GetValueAsString(weaponState);
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, log);
	CurrentWeapon->playChangeMontage(weaponState);
}

void UWeaponComponent::attackHandler()
{
	UE_LOG(LogTemp, Warning, TEXT("weaponComp"));

	CurrentWeapon->playAttackMontage(weaponState, EmouseDirection);
}

void UWeaponComponent::hitActionHandler()
{

}

void UWeaponComponent::dieActionHandler()
{

}
void UWeaponComponent::updateMouseDirection()
{
	// ���� ���콺 ��ġ ��������
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return;

	float MouseX, MouseY;
	PlayerController->GetMousePosition(MouseX, MouseY);
	currentMousePosition = FVector2D(MouseX, MouseY);

	FVector2D MouseDelta = currentMousePosition - lastMousePosition;

	float Threshold = 100.0f;

	if (FMath::Abs(MouseDelta.X) > Threshold || FMath::Abs(MouseDelta.Y) > Threshold)
	{
		if (FMath::Abs(MouseDelta.X) > FMath::Abs(MouseDelta.Y))
		{
			EmouseDirection = (MouseDelta.X > 0) ? EMouseState::RIGHT : EMouseState::LEFT;
		}
		else
		{
			EmouseDirection = (MouseDelta.Y > 0) ? EMouseState::DOWN : EMouseState::UP;
		}
	}
	// ���� ���콺 ��ġ ����
	lastMousePosition = currentMousePosition;
}


