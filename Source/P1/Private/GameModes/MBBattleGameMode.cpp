#include "GameModes/MBBattleGameMode.h"
#include "GameInstances/MBGameInstance.h"
#include "Characters/MBAIBaseCharacter.h"
#include "Characters/MBAISpearman.h"
#include "MBDebug.h"

USkeletalMesh* AMBBattleGameMode::SharedMeshSpearmanPlayerTroop = nullptr;
USkeletalMesh* AMBBattleGameMode::SharedMeshSpearmanEnemyTroop = nullptr;
UStaticMesh* AMBBattleGameMode::SharedMeshSpear = nullptr;
UAnimBlueprint* AMBBattleGameMode::SharedSpearmanAnimBlueprint = nullptr;
UAnimMontage* AMBBattleGameMode::SharedSpearAnimMontage = nullptr;

AMBBattleGameMode::AMBBattleGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BlueTeamMeshObject(TEXT("/Game/YSH/Assets/Infantry/Meshes/SM_AI_PlayerTeam"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> RedTeamMeshObject(TEXT("/Game/YSH/Assets/Infantry/Meshes/SM_AI_EnemyTeam"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObject(TEXT("/Game/YSH/Assets/Infantry/Meshes/SM_Spear"));
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBlueprintObject(TEXT("/Game/YSH/Assets/Anim/ABP_AI"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AnimMontageObject(TEXT("/Game/YSH/Assets/Anim/AM_AI"));

	check(BlueTeamMeshObject.Succeeded());
	check(RedTeamMeshObject.Succeeded());
	check(MeshObject.Succeeded());
	check(AnimBlueprintObject.Succeeded());
	check(AnimMontageObject.Succeeded());

	SharedMeshSpearmanPlayerTroop = BlueTeamMeshObject.Object;
	SharedMeshSpearmanEnemyTroop = RedTeamMeshObject.Object;
	SharedMeshSpear = MeshObject.Object;
	SharedSpearmanAnimBlueprint = AnimBlueprintObject.Object;
	SharedSpearAnimMontage = AnimMontageObject.Object;

	check(SharedMeshSpearmanPlayerTroop);
	check(SharedMeshSpearmanEnemyTroop);
	check(SharedMeshSpear);
	check(SharedSpearmanAnimBlueprint);
	check(SharedSpearAnimMontage);
}

void AMBBattleGameMode::InitGameData()
{
	int InitPlayerTroopCount = 0;
	int InitEnemyTroopCount = 0;

	UMBGameInstance* GameInstance = Cast<UMBGameInstance>(GetGameInstance());

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
				}, 1.f, false);
		}, 1.f, false);
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
	int lineX = 0;
	int lineY = 0;
	for (int32 i = 0; i < InNum; ++i)
	{
		if (i % ColumCount == 0)
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

	Row = lineX;
	Column = ColumCount;

	FString Str = FString::Printf(TEXT("Row = %d, Colum = %d"), Row, Column);
	Debug::Print(*Str);
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
		SpawnedAI->InitCharacter(SharedMeshSpearmanPlayerTroop, SharedMeshSpear, SharedSpearmanAnimBlueprint, SharedSpearAnimMontage, &PlayerTeamInfo.back(), &CharacterStateManager);
		return;
	}
	EnemyTeamInfo.push_back(Info);
	SpawnedAI->InitCharacter(SharedMeshSpearmanEnemyTroop, SharedMeshSpear, SharedSpearmanAnimBlueprint, SharedSpearAnimMontage, &EnemyTeamInfo.back(), &CharacterStateManager);
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

void UpdateForceMoveLocation(AMBAIBaseCharacter* InCharacter, FVector& InLocation)
{
	InCharacter->SetForceMoveLocation(InLocation);
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
		//it->InfoSelfData->Destroy();
		DeadCharacters.pop();
	}
}

void AMBBattleGameMode::DefaultFormation(float InSpace)
{
	FVector StartLocation;
	int FormationRow = 0;
	int ForamtionColumn = 0;

	int FormationCenter = Column / 2;

	auto it = std::next(PlayerTeamInfo.begin(), FormationCenter);
	FVector CenterLocation = it->InfoLocation;

	StartLocation = CenterLocation;
	StartLocation.Y -= InSpace * FormationCenter;

#ifdef DebugMode
	FString TempStr = FString::Printf(TEXT("FormationCenter = %s"), *FString::FromInt(FormationCenter));
	Debug::Print(TempStr);
	TempStr = FString::Printf(TEXT("CenterLocation = %s"), *CenterLocation.ToString());
	Debug::Print(TempStr);
	TempStr = FString::Printf(TEXT("StartLocation = %s"), *StartLocation.ToString());
	Debug::Print(TempStr);
#endif // DebugMode

	for (auto& Data : PlayerTeamInfo)
	{
		Data.InfoSelfData->SetForceMoveLocation(StartLocation + FVector(InSpace * FormationRow, InSpace * ForamtionColumn, 0.f));

		if (ForamtionColumn == Column - 1)
		{
			ForamtionColumn = 0;
			++FormationRow;
		}
		else
		{
			++ForamtionColumn;
		}
	}
}

void AMBBattleGameMode::OrderPlayerTeam(MBOrder* InOrder)
{
	Debug::Print("Ordered!!!");

	for (auto& Data : PlayerTeamInfo)
	{
		Data.InfoSelfData->SetOrder(InOrder);
	}
}

void AMBBattleGameMode::OrderPlayerTeam(MBOrder* InOrder, Formation InFormation)
{
	if (InOrder != &CharacterStateManager.ManagerOrderMakeFormation)
		return;

	for (auto& Data : PlayerTeamInfo)
	{
		Data.InfoSelfData->SetOrder(InOrder);
	}

	SetFormation(InFormation);
}

void AMBBattleGameMode::OrderPlayerTeam(MBOrder* InOrder, Formation InFormation, FVector InLocation)
{
	if (InOrder != &CharacterStateManager.ManagerOrderMoveLocation)
		return;

	for (auto& Data : PlayerTeamInfo)
	{
		Data.InfoSelfData->SetOrder(InOrder);
	}

	SetFormation(InFormation, InLocation);
}

void AMBBattleGameMode::OrderEnemyTeam(MBOrder* InOrder)
{
	for (auto& Data : EnemyTeamInfo)
	{
		Data.InfoSelfData->SetOrder(InOrder);
	}
}

void AMBBattleGameMode::SetFormation(Formation InFormation)
{
	float FormationSpace = static_cast<float>(InFormation);

	FVector StartLocation;
	int FormationRow = 0;
	int ForamtionColumn = 0;

	int FormationCenter = Column / 2;

	auto it = std::next(PlayerTeamInfo.begin(), FormationCenter);
	FVector CenterLocation = it->InfoLocation;

	StartLocation = CenterLocation;
	StartLocation.Y -= FormationSpace * FormationCenter;

#ifdef DebugMode
	FString TempStr = FString::Printf(TEXT("FormationCenter = %s"), *FString::FromInt(FormationCenter));
	Debug::Print(TempStr);
	TempStr = FString::Printf(TEXT("CenterLocation = %s"), *CenterLocation.ToString());
	Debug::Print(TempStr);
	TempStr = FString::Printf(TEXT("StartLocation = %s"), *StartLocation.ToString());
	Debug::Print(TempStr);
#endif // DebugMode

	for (auto& Data : PlayerTeamInfo)
	{
		Data.InfoSelfData->SetForceMoveLocation(StartLocation + FVector(FormationSpace * FormationRow, FormationSpace * ForamtionColumn, 0.f));

		if (ForamtionColumn == Column - 1)
		{
			ForamtionColumn = 0;
			++FormationRow;
		}
		else
		{
			++ForamtionColumn;
		}
	}
}

void AMBBattleGameMode::SetFormation(Formation InFormation, FVector InLocation)
{
	float FormationSpace = static_cast<float>(InFormation);

	FVector StartLocation;
	int FormationRow = 0;
	int ForamtionColumn = 0;

	StartLocation = InLocation;
	StartLocation.Y += FormationSpace / 2;

#ifdef DebugMode
	FString TempStr = FString::Printf(TEXT("StartLocation = %s"), *StartLocation.ToString());
	Debug::Print(TempStr);
#endif // DebugMode

	for (auto& Data : PlayerTeamInfo)
	{
		Data.InfoSelfData->SetForceMoveLocation(StartLocation + FVector(FormationSpace * FormationRow, FormationSpace * ForamtionColumn, 0.f));

		if (ForamtionColumn == Column - 1)
		{
			ForamtionColumn = 0;
			++FormationRow;
		}
		else
		{
			++ForamtionColumn;
		}
	}
}
