// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "WeaponActor.h"
#include "EMouseState.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"
#include "playerWidget.h"
#include "Components/CanvasPanelSlot.h"

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
	widget = CreateWidget<UplayerWidget>(GetWorld(), widgetFactory);
	widget->AddToViewport();

	if (widget==nullptr)return;
	ArrowT = Cast<UImage>(widget->GetWidgetFromName(TEXT("ArrowT")));
	ArrowB = Cast<UImage>(widget->GetWidgetFromName(TEXT("ArrowB")));
	ArrowR = Cast<UImage>(widget->GetWidgetFromName(TEXT("ArrowR")));
	ArrowL = Cast<UImage>(widget->GetWidgetFromName(TEXT("ArrowL")));
	AimT= Cast<UImage>(widget->GetWidgetFromName(TEXT("AimT")));
	AimB= Cast<UImage>(widget->GetWidgetFromName(TEXT("AimB")));
	AimR= Cast<UImage>(widget->GetWidgetFromName(TEXT("AimR")));
	AimL= Cast<UImage>(widget->GetWidgetFromName(TEXT("AimL")));
	AimT->SetOpacity(0);
	AimB->SetOpacity(0);
	AimR->SetOpacity(0);
	AimL->SetOpacity(0);
}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	getWidgetPosition();

	// ...
	updateMouseDirection();
	FString log = UEnum::GetValueAsString(EmouseDirection);
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, log);

	showArrow();
	switch (weaponState)
	{
	case EWeaponState::NONE: { showAim(false); }
		break;
	case EWeaponState::SPEAR: { showAim(false); }
		break;
	case EWeaponState::SWORD: { showAim(false); }
		break;
	case EWeaponState::BOW: { showAim(true); }
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

void UWeaponComponent::attackHandler()
{

	CurrentWeapon->playAttackMontage(weaponState, EmouseDirection);

}

void UWeaponComponent::guardHandler()
{
	CurrentWeapon->playGuardMontage(weaponState, EmouseDirection);

}

void UWeaponComponent::hitActionHandler()
{

}

void UWeaponComponent::dieActionHandler()
{

}
void UWeaponComponent::updateMouseDirection()
{
	// 현재 마우스 위치 가져오기
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return;

	float MouseX, MouseY;
	PlayerController->GetMousePosition(MouseX, MouseY);
	currentMousePosition = FVector2D(MouseX, MouseY);

	FVector2D MouseDelta = currentMousePosition - lastMousePosition;

	float Threshold = 50.0f;

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
	// 이전 마우스 위치 갱신
	lastMousePosition = currentMousePosition;
}

void UWeaponComponent::showAim(bool isAim)
{

	if(isAim){
		AimT->SetOpacity(1); AimB->SetOpacity(1); AimR->SetOpacity(1); AimL->SetOpacity(1);
	}
	else{
		AimT->SetOpacity(0); AimB->SetOpacity(0); AimR->SetOpacity(0); AimL->SetOpacity(0);
	}
}

void UWeaponComponent::showArrow()
{
	if (weaponState == EWeaponState::BOW) { ArrowT->SetOpacity(0); ArrowB->SetOpacity(0); ArrowR->SetOpacity(0); ArrowL->SetOpacity(0); return; }
	switch (EmouseDirection)
	{
	case EMouseState::UP: { ArrowT->SetOpacity(1); ArrowB->SetOpacity(0); ArrowR->SetOpacity(0); ArrowL->SetOpacity(0); }
						break;
	case EMouseState::DOWN: { ArrowT->SetOpacity(0); ArrowB->SetOpacity(1); ArrowR->SetOpacity(0); ArrowL->SetOpacity(0); }
						  break;
	case EMouseState::RIGHT: { ArrowT->SetOpacity(0); ArrowB->SetOpacity(0); ArrowR->SetOpacity(1); ArrowL->SetOpacity(0); }
						   break;
	case EMouseState::LEFT: { ArrowT->SetOpacity(0); ArrowB->SetOpacity(0); ArrowR->SetOpacity(0); ArrowL->SetOpacity(1); }
						  break;
	default:
		break;
	}



}

void UWeaponComponent::getWidgetPosition()
{
	if (AimT&& AimL)
	{
		UCanvasPanelSlot* CanvasSlot1 = Cast<UCanvasPanelSlot>(AimT->Slot);
		UCanvasPanelSlot* CanvasSlot2 = Cast<UCanvasPanelSlot>(AimL->Slot);
		if (CanvasSlot1&& CanvasSlot2)
		{
	;
			UE_LOG(LogTemp, Warning, TEXT("Image Coordinates: X: %f, Y: %f"), CanvasSlot2->GetPosition().X, CanvasSlot1->GetPosition().Y);
		}
	}
}
