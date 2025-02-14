#include "GameModes/MBBattleGameMode.h"
#include "GameInstances/MBGameInstance.h"
#include "Characters/MBAIBaseCharacter.h"
#include "Characters/MBAISpearman.h"
#include "MBDebug.h"

USkeletalMesh* AMBBattleGameMode::SharedMeshSpearmanPlayerTroop = nullptr;
USkeletalMesh* AMBBattleGameMode::SharedMeshSpearmanEnemyTroop = nullptr;
UAnimBlueprint* AMBBattleGameMode::SharedSpearmanAnimBlueprint = nullptr;

AMBBattleGameMode::AMBBattleGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BlueTeamMeshObject(TEXT("/Game/YSH/Assets/Mesh/SK_Blue"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> RedTeamMeshObject(TEXT("/Game/YSH/Assets/Mesh/SK_Red"));
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBlueprintObj(TEXT("/Game/YSH/Assets/Anim/ABP_SpearMan"));

	check(BlueTeamMeshObject.Succeeded());
	check(RedTeamMeshObject.Succeeded());
	check(AnimBlueprintObj.Succeeded());

	SharedMeshSpearmanPlayerTroop = BlueTeamMeshObject.Object;
	SharedMeshSpearmanEnemyTroop = RedTeamMeshObject.Object;
	SharedSpearmanAnimBlueprint = AnimBlueprintObj.Object;

	check(SharedMeshSpearmanPlayerTroop);
	check(SharedMeshSpearmanEnemyTroop);
	check(SharedSpearmanAnimBlueprint);
}

void AMBBattleGameMode::InitGameData()
{
	int InitPlayerTroopCount = 0;
	int InitEnemyTroopCount = 0;

	UMBGameInstance* GameInstance = Cast<UMBGameInstance>(GetGameInstance());

#ifdef DebugMode
	if (false == Debug::NullCheck(GameInstance, "GameInstance"))
		__debugbreak();
#endif // DebugMode

	GameInstance->LoadBattleData(InitPlayerTroopCount, InitEnemyTroopCount);

	FVector PlayerTeamLocation(0.f, 100.f, 100.f);
	FRotator PlayerTeamRotation(0.f, 180.f, 0.f);
	FVector EnemyTeamLocation(-4000.f, 1000.f, 100.f);
	FRotator EnemyTeamRotation(0.f, 0.f, 0.f);

	BattleInitSpawn(true, InitPlayerTroopCount, PlayerTeamLocation, PlayerTeamRotation);
	BattleInitSpawn(false, InitEnemyTroopCount, EnemyTeamLocation, EnemyTeamRotation);
}

void AMBBattleGameMode::BeginPlay()
{
#ifdef DebugMode
	Debug::Called();
#endif // DebugMode

	InitGameData();

	UpdateTeamCount();
	UpdateAllCharacterInfo();
	UpdateTargets();

	CachedWorld = GetWorld();

	// Debug
	CachedWorld->GetTimerManager().SetTimer(DebugTimer, [this]()
		{
			this->OrderPlayerTeam(&CharacterStateManager.ManagerOrderHoldPosition);
			this->OrderEnemyTeam(&CharacterStateManager.ManagerOrderHoldPosition);
			Debug::Print("Order: HoldPosition");

			CachedWorld->GetTimerManager().SetTimer(DebugTimer, [this]()
				{
					this->OrderPlayerTeam(&CharacterStateManager.ManagerOrderEngageBattle);
					this->OrderEnemyTeam(&CharacterStateManager.ManagerOrderEngageBattle);
					Debug::Print("Order: EngageBattle");

					CachedWorld->GetTimerManager().SetTimer(DebugTimer, [this]()
						{
							this->OrderPlayerTeam(&CharacterStateManager.ManagerOrderHoldPosition);
							//this->OrderEnemyTeam(&CharacterStateManager.ManagerOrderHoldPosition);
							Debug::Print("Order: HoldPosition");

							CachedWorld->GetTimerManager().SetTimer(DebugTimer, [this]()
								{
									this->OrderPlayerTeam(&CharacterStateManager.ManagerOrderEngageBattle);
									//this->OrderEnemyTeam(&CharacterStateManager.ManagerOrderEngageBattle);
									Debug::Print("Order: EngageBattle");
								}, 3.f, false);
						}, 3.f, false);
				}, 5.f, false);
		}, 5.f, false);
}

void AMBBattleGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateTeamCount();
	UpdateAllCharacterInfo();

	if (0 < PlayerTeamCount || 0 < EnemyTeamCount)
	{
		UpdateTargets();

		SearchDeadCharacter(PlayerTeamInfo);
		SearchDeadCharacter(EnemyTeamInfo);
	}
}

void AMBBattleGameMode::BattleInitSpawn(bool InIsPlayerTeam, int32 InNum, FVector InLocation, FRotator InRotation)
{
	int32 lineX = 0;
	int32 lineY = 0;
	for (int32 i = 0; i < InNum; ++i)
	{
		if (i % 50 == 0)
		{
			if (InIsPlayerTeam)
			{
				++lineX;
			}
			else
			{
				--lineX;
			}
			lineY = 0;
		}

		++lineY;
		FVector SpawnLocation = InLocation + FVector(lineX * 300.0f, lineY * 200.f, 0.f);
		SpawnCharacter(InIsPlayerTeam, SpawnLocation, InRotation);
	}
}

void AMBBattleGameMode::SpawnCharacter(bool InIsPlayerTeam, FVector InLocation, FRotator InRotation)
{
	AMBAISpearman* SpawnedAI = GetWorld()->SpawnActor<AMBAISpearman>(AMBAISpearman::StaticClass(), InLocation, InRotation);
	SpawnedAI->PrimaryActorTick.AddPrerequisite(this, this->PrimaryActorTick);

	AIInfoData Info;
	Info.InfoSelfData = SpawnedAI;

	if (InIsPlayerTeam)
	{
		PlayerTeamInfo.push_back(Info);
		SpawnedAI->InitCharacter(SharedMeshSpearmanPlayerTroop, SharedSpearmanAnimBlueprint, &PlayerTeamInfo.back(), &CharacterStateManager);
		return;
	}
	EnemyTeamInfo.push_back(Info);
	SpawnedAI->InitCharacter(SharedMeshSpearmanEnemyTroop, SharedSpearmanAnimBlueprint, &EnemyTeamInfo.back(), &CharacterStateManager);
}

void AMBBattleGameMode::UpdateTeamCount()
{
	PlayerTeamCount = PlayerTeamInfo.size();
	EnemyTeamCount = EnemyTeamInfo.size();
}

void AMBBattleGameMode::UpdateAllCharacterInfo()
{
	for (auto& InfoData : PlayerTeamInfo)
	{
		InfoData.InfoLocation = InfoData.InfoSelfData->GetActorLocation();
		InfoData.InfoRotation = InfoData.InfoSelfData->GetActorRotation();
	}

	for (auto& InfoData : EnemyTeamInfo)
	{
		InfoData.InfoLocation = InfoData.InfoSelfData->GetActorLocation();
		InfoData.InfoRotation = InfoData.InfoSelfData->GetActorRotation();
	}
}

void AMBBattleGameMode::UpdateTargets()
{
	auto PlayerTeamInfoData = PlayerTeamInfo.begin();
	auto EnemyTeamInfoData = EnemyTeamInfo.begin();

	bool ReadPlayerTeamInfo = true;
	bool ReadEnemyTeamInfo = true;

	while (ReadPlayerTeamInfo)
	{
		PlayerTeamInfoData->InfoTargetData = EnemyTeamInfoData->InfoSelfData;

		++PlayerTeamInfoData;
		if (PlayerTeamInfoData == PlayerTeamInfo.end())
			ReadPlayerTeamInfo = false;
		++EnemyTeamInfoData;
		if (EnemyTeamInfoData == EnemyTeamInfo.end())
			EnemyTeamInfoData = EnemyTeamInfo.begin();
	}

	PlayerTeamInfoData = PlayerTeamInfo.begin();
	EnemyTeamInfoData = EnemyTeamInfo.begin();

	while (ReadEnemyTeamInfo)
	{
		EnemyTeamInfoData->InfoTargetData = PlayerTeamInfoData->InfoSelfData;

		++EnemyTeamInfoData;
		if (EnemyTeamInfoData == EnemyTeamInfo.end())
			ReadEnemyTeamInfo = false;
		++PlayerTeamInfoData;
		if (PlayerTeamInfoData == PlayerTeamInfo.end())
			PlayerTeamInfoData = PlayerTeamInfo.begin();
	}
}

void AMBBattleGameMode::SearchDeadCharacter(std::list<AIInfoData>& InData)
{
	for (auto it = InData.begin(); it != InData.end(); ++it)
	{
		if (true == it->InfoSelfData->GetIsDead())
		{
			DeadCharacters.push(it);
		}
	}

	while (!DeadCharacters.empty())
	{
		auto it = DeadCharacters.top();
		InData.erase(it);
		it->InfoSelfData->Destroy();
		DeadCharacters.pop();
	}
}

void AMBBattleGameMode::OrderPlayerTeam(MBOrder* InOrder)
{
	for (auto& Data : PlayerTeamInfo)
	{
		Data.InfoSelfData->SetOrder(InOrder);
	}
}

void AMBBattleGameMode::OrderEnemyTeam(MBOrder* InOrder)
{
	for (auto& Data : EnemyTeamInfo)
	{
		Data.InfoSelfData->SetOrder(InOrder);
	}
}
