// Fill out your copyright notice in the Description page of Project Settings.


#include "playerWidget.h"
#include "Animation/WidgetAnimation.h"
#include "GameModes/MBBattleGameMode.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"


void UplayerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AnimDelegate.BindDynamic(this, &UplayerWidget::PlayAnimReverse);
	BindToAnimationFinished(AimAnimation, AnimDelegate);

	gamemode = Cast<AMBBattleGameMode>(UGameplayStatics::GetGameMode(GetWorld()->GetAuthGameMode()));



}

void UplayerWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);
	if(AimAnimation)
		CurrentAnimationTime = this->GetAnimationCurrentTime(AimAnimation);

	int playerTeamCount= gamemode->PlayerTeamCount;
	int enemyTeamCount = gamemode->EnemyTeamCount;

	playerTeam->SetPercent(FMath::GetRangePct(0.0f, 200.0f, playerTeamCount));
	enemyTeam->SetPercent(FMath::GetRangePct(0.0f, 200.0f, enemyTeamCount));
}

void UplayerWidget::AimPlayAnimation(bool isClick)
{
	isArrowClick = isClick;
	if (isClick) {
		bFinished = false;
		PlayAnimation(AimAnimation, 0.0f, 1.0f, EUMGSequencePlayMode::Forward, 1.0f);// 정방향
	}
	else
	{

		PlayAnimation(AimAnimationReturn, 0.0f, 1.0f, EUMGSequencePlayMode::Forward, 1.0f);  // 역방향
	}


}

void UplayerWidget::PlayAnimReverse()
{
	if (!bFinished)
	{
		if (!isArrowClick) return;
		PlayAnimation(AimOut, 0.0f, 1.0f, EUMGSequencePlayMode::Forward, 1.0f);
		bFinished = true;
	}
}

