#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MBGameInstance.generated.h"

UCLASS()
class P1_API UMBGameInstance : public UGameInstance
{
	GENERATED_BODY()

public: // Init
	UMBGameInstance();
	void InitData();

public: // Called Outside
	void LoadBattleData(int& InPlayerData, int& InEnemyData);

private: // Player Data
	int PlayerTroopCount;

private: // Gobal Game Data
	int CurrentBattleEnemyTroopCount;
};
