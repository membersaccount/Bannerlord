#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <list>
#include <utility>
#include <stack>
#include "MBSettings.h"
#include "Datas/MBStructs.h"
#include "MBBattleGameMode.generated.h"

UCLASS()
class P1_API AMBBattleGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	typedef Structs::AI::InfoData::AIInfoData AIInfoData;

public:
	AMBBattleGameMode();
	void InitGameData();

protected: // Default
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private: // Spawn
	void BattleInitSpawn(bool InIsPlayerTeam, int32 InNum, FVector InLocation, FRotator InRotation);
	void SpawnCharacter(bool InIsPlayerTeam, FVector InLocation, FRotator InRotation);

private: // Update
	void UpdateTeamCount();
	void UpdateAllCharacterInfo();
	void UpdateTargets();

private: // Character Manage
	void SearchDeadCharacter(std::list<AIInfoData>& InData);

private: // Shared Data
	static USkeletalMesh* SharedMeshSpearmanPlayerTroop;
	static USkeletalMesh* SharedMeshSpearmanEnemyTroop;
	static UAnimBlueprint* SharedSpearmanAnimBlueprint;

private: // Character Data
	int PlayerTeamCount;
	int EnemyTeamCount;
	std::list<AIInfoData> PlayerTeamInfo;
	std::list<AIInfoData> EnemyTeamInfo;

	std::stack<std::list<AIInfoData>::iterator> DeadCharacters;

private: // Cached Data
	UWorld* CachedWorld;
};
