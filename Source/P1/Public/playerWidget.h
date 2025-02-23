// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "playerWidget.generated.h"

/**
 *
 */
UCLASS()
class P1_API UplayerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

public:
	//애니매이션 역재생 여부
	bool isArrowClick;
	//애니메이션 시간 저장 변수
	float CurrentAnimationTime;
public:
	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,BlueprintReadWrite)
    UWidgetAnimation* AimAnimation;
	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,BlueprintReadWrite)
    UWidgetAnimation* AimAnimationReturn;

	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,BlueprintReadWrite)
    UWidgetAnimation* AimOut;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* playerTeam;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* enemyTeam;


	class AMBBattleGameMode* gamemode;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HPUI;

	//에임 애니메이션 재생 함수
	void AimPlayAnimation(bool isClick);

	bool bFinished = false;

	FWidgetAnimationDynamicEvent AnimDelegate;
	UFUNCTION()
	void PlayAnimReverse();
};
