// Fill out your copyright notice in the Description page of Project Settings.


#include "playerWidget.h"
#include "Animation/WidgetAnimation.h"


void UplayerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AnimDelegate.BindDynamic(this, &UplayerWidget::PlayAnimReverse);
	BindToAnimationFinished(AimAnimation, AnimDelegate);


}

void UplayerWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);
	//if(AimAnimation)
	//CurrentAnimationTime = this->GetAnimationCurrentTime(AimAnimation);
}

void UplayerWidget::AimPlayAnimation(bool isClick)
{
	isArrowClick = isClick;
	if (isClick) {
		bFinished = false;
		PlayAnimation(AimAnimation, 0.0f, 1.0f, EUMGSequencePlayMode::Forward, 1.0f);// 沥规氢
	}
	else
	{

		PlayAnimation(AimAnimation, 0.0f, 1.0f, EUMGSequencePlayMode::Reverse, 1.0f);  // 开规氢
	}

}

void UplayerWidget::PlayAnimReverse()
{
	if (!bFinished)
	{
		PlayAnimation(AimOut, 0.0f, 1.0f, EUMGSequencePlayMode::Reverse, 1.0f); // 开规氢
		bFinished = true;
	}
}

