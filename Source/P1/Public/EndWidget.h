// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndWidget.generated.h"

/**
 *
 */
UCLASS()
class P1_API UEndWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,BlueprintReadWrite)
	class UWidgetAnimation* uidown;

};
