#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundAttenuation.h"
#include "MBGameInstance.generated.h"

UCLASS()
class P1_API UMBGameInstance : public UGameInstance
{
	GENERATED_BODY()

public: // Init
	UMBGameInstance();
	void InitData();
	void AnimNotify_footSound();
	UFUNCTION(BlueprintCallable, Category = "Animation")
	FVector GetAnimationActorLocation() const;

public: // Called Outside
	void LoadBattleData(int& InPlayerData, int& InEnemyData);
	USoundCue* foodSound;
	USoundAttenuation* AttenuationSettings;

private: // Player Data
	int PlayerTroopCount;

private: // Gobal Game Data
	int CurrentBattleEnemyTroopCount;
};
