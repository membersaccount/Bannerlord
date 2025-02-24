#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <list>
#include <utility>
#include <stack>
#include <unordered_map>
#include "MBSettings.h"
#include "AI/MBStateManager.h"
#include "Datas/MBStructs.h"
#include "Datas/MBEnums.h"
#include "MBBattleGameMode.generated.h"

UCLASS()
class P1_API AMBBattleGameMode : public AGameModeBase
{
	GENERATED_BODY()

	typedef Structs::AI::AIInfoData AIInfoData;
	typedef Enums::Player::Order::Formation Formation;

public:
	AMBBattleGameMode();
	void InitGameData();

protected: // Default
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public: // Player Troop Controll
	void OrderPlayerTeam(MBOrder* InOrder);
	void OrderPlayerTeam(MBOrder* InOrder, Formation InFormation);
	void OrderPlayerTeam(MBOrder* InOrder, Formation InFormation, FVector InLocation);
	void OrderEnemyTeam(MBOrder* InOrder);
	void SetFormation(Formation InFormation);
	void SetFormation(Formation InFormation, FVector InLocation);

	void DefaultFormation(float InSpace);
	MBStateManager CharacterStateManager;

	int PlayerTeamCount;
	int EnemyTeamCount;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class UUserWidget>introWidget;
	class UUserWidget* widget;

public: // Target Search
	int TargetSearchCloseTeam(AMBAIBaseCharacter* InCharacter);
	void SearchCloseTarget(AMBAIBaseCharacter* InCharacter, int InTargetIndex);

private: // Spawn
	void BattleInitSpawn(bool InIsPlayerTeam, int32 InNum, FVector InLocation, FRotator InRotation);
	void SpawnCharacter(bool InIsPlayerTeam, FVector InLocation, FRotator InRotation, int InPlayerTroopTeam);

private: // Update
	void UpdateTeamCount();
	void UpdateAllCharacterInfo();
	void UpdateTargets();
	void SearchTargets();
	void UpdateForceMoveLocation(AMBAIBaseCharacter* InCharacter, FVector& InLocation);

	void UpdateTroopTeamCenter();

	float CalculateDistance(FVector& InLocation, FVector& InTargetLocation);

private: // Character Manage
	void SearchDeadCharacter(std::list<AIInfoData>& InData);

private: // Shared Data
	static USkeletalMesh* SharedMeshSpearmanPlayerTroop;
	static USkeletalMesh* SharedMeshSpearmanEnemyTroop;
	static UStaticMesh* SharedMeshSpear;
	static UAnimBlueprint* SharedSpearmanAnimBlueprint;
	static UAnimMontage* SharedSpearMontageFullbody;
	static UAnimMontage* SharedSpearMontageUpperbody;

private: // Character Data
	std::list<AIInfoData> PlayerTeamInfo;
	std::list<AIInfoData> EnemyTeamInfo;
	std::vector<std::list<AIInfoData*>> PlayerTroopTeam;
	std::vector<std::list<AIInfoData*>> EnemyTroopTeam;
	std::vector<FVector> PlayerTroopTeamLocation;
	std::vector<FVector> EnemyTroopTeamLocation;
	std::vector<std::vector<std::pair<int,float>>> PlayerTeamDistance;
	std::vector<std::vector<std::pair<int,float>>> EnemyTeamDistance;
	std::stack<std::list<AIInfoData>::iterator> DeadCharacters;
	std::stack<std::list<AIInfoData>::iterator> TargetUpdate;

private: // Formation
	int Row = 0;	// Y
	int Column = 0;	// X

private: // Cached Data
	UWorld* CachedWorld;

private: // Debug
	FTimerHandle DebugTimer;
	int ColumCount = 40;
	bool IsGameEnd = false;

#ifdef DebugMode
public:
	void ShowTeam();
	void ShowTroopTeamCenterLocation();
	void ShowTroopTeamRectangle(float InMaxX, float InMinX, float InMaxY, float InMinY, FColor InColor);
#endif // DebugMode
};
