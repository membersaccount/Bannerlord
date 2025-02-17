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
	//�ִϸ��̼� ����� ����
	bool isArrowClick;
	//�ִϸ��̼� �ð� ���� ����
	float CurrentAnimationTime;
public:
	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,BlueprintReadWrite)
    UWidgetAnimation* AimAnimation;

		UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,BlueprintReadWrite)
    UWidgetAnimation* AimOut;

	//���� �ִϸ��̼� ��� �Լ�
	void AimPlayAnimation(bool isClick);

	bool bFinished = false;

	FWidgetAnimationDynamicEvent AnimDelegate;
	UFUNCTION()
	void PlayAnimReverse();
};
