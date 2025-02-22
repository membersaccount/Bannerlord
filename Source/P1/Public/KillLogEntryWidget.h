// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillLogEntryWidget.generated.h"

/**
 *
 */
UCLASS()
class P1_API UKillLogEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* KillLogText;

	UPROPERTY(meta = (BindWidget))
	class UImage* KillLogImage;

};
