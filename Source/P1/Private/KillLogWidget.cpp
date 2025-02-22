// Fill out your copyright notice in the Description page of Project Settings.


#include "KillLogWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Blueprint/UserWidget.h"
#include "KillLogEntryWidget.h"

void UKillLogWidget::AddKillLogEntry(const FText& KillMessage)
{


	UKillLogEntryWidget* NewEntryWidget = CreateWidget<UKillLogEntryWidget>(this, KillLogEntryClass);
	if (NewEntryWidget)
	{
		UTextBlock* KillLogText = Cast<UTextBlock>(NewEntryWidget->KillLogText);
		if (KillLogText)
		{
			KillLogText->SetText(KillMessage);
			KillLogBox->AddChildToVerticalBox(NewEntryWidget);

			FTimerHandle visibleTime;
			FTimerDelegate TimerLambda = FTimerDelegate::CreateLambda([this]() { ClearKillLog(); });
			GetWorld()->GetTimerManager().SetTimer(visibleTime, TimerLambda, 1.0f, false);
		}
	}
}

void UKillLogWidget::ClearKillLog()
{
	if (KillLogBox)
	{
		KillLogBox->RemoveChildAt(0);
	}
}
