#include "GameModes/MBBattleGameMode.h"
#include "GameInstances/MBGameInstance.h"
#include "Characters/MBAIBaseCharacter.h"
#include "Characters/MBAISpearman.h"
#include "MBDebug.h"

USkeletalMesh* AMBBattleGameMode::SharedMeshSpearmanPlayerTroop = nullptr;
USkeletalMesh* AMBBattleGameMode::SharedMeshSpearmanEnemyTroop = nullptr;
UStaticMesh* AMBBattleGameMode::SharedMeshSpear = nullptr;
UAnimBlueprint* AMBBattleGameMode::SharedSpearmanAnimBlueprint = nullptr;
UAnimMontage* AMBBattleGameMode::SharedSpearmanAttackMontage = nullptr;

AMBBattleGameMode::AMBBattleGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BlueTeamMeshObject(TEXT("/Game/YSH/Assets/Infantry/Meshes/MFS_vA"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> RedTeamMeshObject(TEXT("/Game/YSH/Assets/Mesh/SK_Red"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObject(TEXT("/Game/YSH/Assets/Infantry/Meshes/SM_Spear"));
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBlueprintObj(TEXT("/Game/YSH/Assets/Anim/ABP_AI"));
	//static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageSpearAttackObject(TEXT("/Game/YSH/Assets/Anim/AM_AI"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageSpearAttackObject(TEXT("/Game/LHW/ReTargeting/SpearMontage/Spear_RP_AttackFwd_Swing_topHeavy_Montage1"));

	check(BlueTeamMeshObject.Succeeded());
	check(RedTeamMeshObject.Succeeded());
	check(MeshObject.Succeeded());
	check(AnimBlueprintObj.Succeeded());
	check(MontageSpearAttackObject.Succeeded());

	SharedMeshSpearmanPlayerTroop = BlueTeamMeshObject.Object;
	SharedMeshSpearmanEnemyTroop = RedTeamMeshObject.Object;
	SharedMeshSpear = MeshObject.Object;
	SharedSpearmanAnimBlueprint = AnimBlueprintObj.Object;
	SharedSpearmanAttackMontage = MontageSpearAttackObject.Object;

	check(SharedMeshSpearmanPlayerTroop);
	check(SharedMeshSpearmanEnemyTroop);
	check(SharedMeshSpear);
	check(SharedSpearmanAnimBlueprint);
	check(SharedSpearmanAttackMontage);
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
					//this->OrderEnemyTeam(&CharacterStateManager.ManagerOrderEngageBattle);
					Debug::Print("Order: EngageBattle");
				}, 1.f, false);

			//CachedWorld->GetTimerManager().SetTimer(DebugTimer, [this]()
			//	{
			//		this->DefaultFormation(200.f);
			//		this->OrderPlayerTeam(&CharacterStateManager.ManagerOrderMakeFormation);
			//		Debug::Print("Order: MakeFormation - 200");

			//		CachedWorld->GetTimerManager().SetTimer(DebugTimer, [this]()
			//			{
			//				this->DefaultFormation(500.f);
			//				this->OrderPlayerTeam(&CharacterStateManager.ManagerOrderMakeFormation);
			//				Debug::Print("Order: MakeFormation - 500");

			//				CachedWorld->GetTimerManager().SetTimer(DebugTimer, [this]()
			//					{
			//						this->DefaultFormation(300.f);
			//						this->OrderPlayerTeam(&CharacterStateManager.ManagerOrderMakeFormation);
			//						Debug::Print("Order: MakeFormation - 300");

			//						CachedWorld->GetTimerManager().SetTimer(DebugTimer, [this]()
			//							{
			//								this->OrderPlayerTeam(&CharacterStateManager.ManagerOrderEngageBattle);
			//								this->OrderEnemyTeam(&CharacterStateManager.ManagerOrderEngageBattle);
			//								Debug::Print("Order: EngageBattle");

			//								CachedWorld->GetTimerManager().SetTimer(DebugTimer, [this]()
			//									{
			//										this->OrderPlayerTeam(&CharacterStateManager.ManagerOrderHoldPosition);
			//										//this->OrderEnemyTeam(&CharacterStateManager.ManagerOrderHoldPosition);
			//										Debug::Print("Order: HoldPosition");

			//										CachedWorld->GetTimerManager().SetTimer(DebugTimer, [this]()
			//											{
			//												this->OrderPlayerTeam(&CharacterStateManager.ManagerOrderEngageBattle);
			//												//this->OrderEnemyTeam(&CharacterStateManager.ManagerOrderEngageBattle);
			//												Debug::Print("Order: EngageBattle");
			//											}, 3.f, false);
			//									}, 3.f, false);
			//							}, 0.5f, false);
			//					}, 0.5f, false);
			//			}, 0.5f, false);
			//	}, 0.5f, false);
		}, 0.9f, false);
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

	Row = lineX;
	Column = 3;

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
		SpawnedAI->InitCharacter(SharedMeshSpearmanPlayerTroop, SharedMeshSpear, SharedSpearmanAnimBlueprint, SharedSpearmanAttackMontage, &PlayerTeamInfo.back(), &CharacterStateManager);
		return;
	}
	EnemyTeamInfo.push_back(Info);
	SpawnedAI->InitCharacter(SharedMeshSpearmanEnemyTroop, SharedMeshSpear, SharedSpearmanAnimBlueprint, SharedSpearmanAttackMontage, &EnemyTeamInfo.back(), &CharacterStateManager);
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
		it->InfoSelfData->Destroy();
		DeadCharacters.pop();
	}
}

void AMBBattleGameMode::DefaultFormation(float InSpace)
{
	FVector StartLocation;
	int ForamtionColumn = 0;
	int FormationRow = 0;
	int Count = 1;

	//if (1 == Row % 2)
	//{
	//	int FormationCenter = (Column * 2 - 1) * Row + (Row / 2 + 1);

	//	auto it = std::next(PlayerTeamInfo.begin(), FormationCenter);
	//	FVector CenterLocation = it->InfoLocation;
	//
	//	StartLocation = CenterLocation + FVector(-InSpace * (Row / 2), -InSpace * (Column / 2), 0.f);

	//}

	int FormationCenter = (Column * 2 - 1) * Row + (Row / 2 + 1);

	auto it = std::next(PlayerTeamInfo.begin(), FormationCenter);
	FVector CenterLocation = it->InfoLocation;

	StartLocation = CenterLocation + FVector(-InSpace * (Row / 2), -InSpace * (Column / 2), 0.f);

	for (auto& Data : PlayerTeamInfo)
	{
		Data.InfoSelfData->SetForceMoveLocation(StartLocation + FVector(FormationRow * InSpace, ForamtionColumn * InSpace, 0.f));

		if (Count % 50 == 0)
		{
			++FormationRow;
			ForamtionColumn = 0;
		}
		else
		{
			++ForamtionColumn;
		}
		++Count;
	}
}

void AMBBattleGameMode::SetFormation()
{
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
