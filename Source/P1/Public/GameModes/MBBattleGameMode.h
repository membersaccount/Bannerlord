#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <list>
#include <utility>
#include <stack>
#include "MBSettings.h"
#include "AI/MBStateManager.h"
#include "Datas/MBStructs.h"
#include "MBBattleGameMode.generated.h"

UCLASS()
class P1_API AMBBattleGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	typedef Structs::AI::AIInfoData AIInfoData;

public:
	AMBBattleGameMode();
	void InitGameData();

public:
	void OrderPlayerTeam(MBOrder* InOrder);
	void OrderEnemyTeam(MBOrder* InOrder);
	void DefaultFormation(float InSpace);

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
	void UpdateForceMoveLocation(AMBAIBaseCharacter* InCharacter, FVector& InLocation);

private: // Character Manage
	void SearchDeadCharacter(std::list<AIInfoData>& InData);

private: // Formation
	void SetFormation();

private: // Shared Data
	static USkeletalMesh* SharedMeshSpearmanPlayerTroop;
	static USkeletalMesh* SharedMeshSpearmanEnemyTroop;
	static UStaticMesh* SharedMeshSpear;
	static UAnimBlueprint* SharedSpearmanAnimBlueprint;
	static UAnimMontage* SharedSpearAnimMontage;

private: // Character Data
	int PlayerTeamCount;
	int EnemyTeamCount;
	std::list<AIInfoData> PlayerTeamInfo;
	std::list<AIInfoData> EnemyTeamInfo;
	MBStateManager CharacterStateManager;

	std::stack<std::list<AIInfoData>::iterator> DeadCharacters;

private: // Formation
	int Row = 0;	// Y
	int Column = 0;	// X

private: // Cached Data
	UWorld* CachedWorld;

private: // Debug
	FTimerHandle DebugTimer;
	int ColumCount = 50;
};
