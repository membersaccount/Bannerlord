#include "GameInstances/MBGameInstance.h"
#include "MBSettings.h"

UMBGameInstance::UMBGameInstance()
{
	InitData();
}

void UMBGameInstance::InitData()
{
	PlayerTroopCount = Data::AI::Testing::PlayerTroopTestCount;
	CurrentBattleEnemyTroopCount = Data::AI::Testing::EnemyTroopTestCount;
}

void UMBGameInstance::LoadBattleData(int& InPlayerData, int& InEnemyData)
{
	InPlayerData = PlayerTroopCount;
	InEnemyData = CurrentBattleEnemyTroopCount;
}

