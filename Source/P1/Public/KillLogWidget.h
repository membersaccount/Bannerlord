// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillLogWidget.generated.h"

/**
 *
 */
UCLASS()
class P1_API UKillLogWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* KillLogBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kill Log")
	TSubclassOf<class UKillLogEntryWidget> KillLogEntryClass;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kill Log")
	TSubclassOf<class UKillLogEntryWidget> KillLogEntryClass1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kill Log")
	int32 MaxEntries;

	UFUNCTION(BlueprintCallable, Category = "Kill Log")
	void AddKillLogEntry(const FText& KillMessage,int isenemyState);

	UFUNCTION(BlueprintCallable, Category = "Kill Log")
	void ClearKillLog();

};
