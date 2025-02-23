#include "GameModes/MBBattleGameMode.h"
#include "GameInstances/MBGameInstance.h"
#include "Characters/MBAIBaseCharacter.h"
#include "Characters/MBAISpearman.h"
#include "MBDebug.h"
#include "Components/Widget.h"
#include "Blueprint/UserWidget.h"

USkeletalMesh* AMBBattleGameMode::SharedMeshSpearmanPlayerTroop = nullptr;
USkeletalMesh* AMBBattleGameMode::SharedMeshSpearmanEnemyTroop = nullptr;
UStaticMesh* AMBBattleGameMode::SharedMeshSpear = nullptr;
UAnimBlueprint* AMBBattleGameMode::SharedSpearmanAnimBlueprint = nullptr;
UAnimMontage* AMBBattleGameMode::SharedSpearMontageFullbody = nullptr;
UAnimMontage* AMBBattleGameMode::SharedSpearMontageUpperbody = nullptr;

AMBBattleGameMode::AMBBattleGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BlueTeamMeshObject(TEXT("/Game/YSH/Assets/Infantry/Meshes/SM_AI_PlayerTeam"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> RedTeamMeshObject(TEXT("/Game/YSH/Assets/Infantry/Meshes/SM_AI_EnemyTeam"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObject(TEXT("/Game/YSH/Assets/Infantry/Meshes/SM_Spear"));
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBlueprintObject(TEXT("/Game/YSH/Assets/Anim/ABP_AI"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SpearMontageFullBodyObject(TEXT("/Game/YSH/Assets/Anim/AM_AI_Full"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SpearMontageUpperBodyObject(TEXT("/Game/YSH/Assets/Anim/AM_AI_Upper"));

	check(BlueTeamMeshObject.Succeeded());
	check(RedTeamMeshObject.Succeeded());
	check(MeshObject.Succeeded());
	check(AnimBlueprintObject.Succeeded());
	check(SpearMontageFullBodyObject.Succeeded());
	check(SpearMontageUpperBodyObject.Succeeded());

	SharedMeshSpearmanPlayerTroop = BlueTeamMeshObject.Object;
	SharedMeshSpearmanEnemyTroop = RedTeamMeshObject.Object;
	SharedMeshSpear = MeshObject.Object;
	SharedSpearmanAnimBlueprint = AnimBlueprintObject.Object;
	SharedSpearMontageFullbody = SpearMontageFullBodyObject.Object;
	SharedSpearMontageUpperbody = SpearMontageUpperBodyObject.Object;

	check(SharedMeshSpearmanPlayerTroop);
	check(SharedMeshSpearmanEnemyTroop);
	check(SharedMeshSpear);
	check(SharedSpearmanAnimBlueprint);
	check(SharedSpearMontageFullbody);
	check(SharedSpearMontageUpperbody);
}

void AMBBattleGameMode::InitGameData()
{
	int InitPlayerTroopCount = 0;
	int InitEnemyTroopCount = 0;

	PlayerTroopTeam.resize(20, std::list<AIInfoData*>());
	EnemyTroopTeam.resize(20, std::list<AIInfoData*>());
	PlayerTroopTeamLocation.resize(20);
	EnemyTroopTeamLocation.resize(20);

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

	widget = CreateWidget<UUserWidget>(GetWorld(),introWidget);
	widget->AddToViewport();


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
#ifdef DebugMode
			Debug::Print("Order: HoldPosition");
#endif // DebugMode
		}, 1.f, false);
}

void AMBBattleGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SearchDeadCharacter(PlayerTeamInfo);
	SearchDeadCharacter(EnemyTeamInfo);
	UpdateTeamCount();

	if (0 < PlayerTeamCount && 0 < EnemyTeamCount)
	{
		UpdateAllCharacterInfo();
		UpdateTargets();
		UpdateTroopTeamCenter();
	}
	else
	{
		if (false == IsGameEnd)
		{
			OrderPlayerTeam(&CharacterStateManager.ManagerOrderForceStop);
			OrderEnemyTeam(&CharacterStateManager.ManagerOrderForceStop);

			IsGameEnd = true;
		}
	}

#ifdef DebugMode
	//ShowTeam();
	ShowTroopTeamCenterLocation();
#endif // DebugMode

}

void AMBBattleGameMode::TargetSearchCloseTeam()
{

}

void AMBBattleGameMode::BattleInitSpawn(bool InIsPlayerTeam, int32 InNum, FVector InLocation, FRotator InRotation)
{
	int lineX = 0;
	int lineY = 0;
	int TroopTeamCount = 0;

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
			TroopTeamCount = 0;
		}

		++lineY;
		FVector SpawnLocation = InLocation + FVector(lineX * 300.0f, lineY * 200.f, 0.f);
		SpawnCharacter(InIsPlayerTeam, SpawnLocation, InRotation, TroopTeamCount / 2);
		++TroopTeamCount;
	}

	Row = lineX;
	Column = ColumCount;

#ifdef DebugMode
	FString Str = FString::Printf(TEXT("Row = %d, Colum = %d"), Row, Column);
	Debug::Print(*Str);
#endif // DebugMode
}

void AMBBattleGameMode::SpawnCharacter(bool InIsPlayerTeam, FVector InLocation, FRotator InRotation, int InTroopTeam)
{
	AMBAISpearman* SpawnedAI = GetWorld()->SpawnActor<AMBAISpearman>(AMBAISpearman::StaticClass(), InLocation, InRotation);
	SpawnedAI->PrimaryActorTick.AddPrerequisite(this, this->PrimaryActorTick);

	AIInfoData Info;
	Info.InfoSelfData = SpawnedAI;

	if (InIsPlayerTeam)
	{
		PlayerTeamInfo.push_back(Info);
		SpawnedAI->InitCharacter(SharedMeshSpearmanPlayerTroop, SharedMeshSpear, SharedSpearmanAnimBlueprint, SharedSpearMontageFullbody, SharedSpearMontageUpperbody , &PlayerTeamInfo.back(), &CharacterStateManager, InIsPlayerTeam);
		PlayerTroopTeam[InTroopTeam].push_back(&PlayerTeamInfo.back());
		return;
	}
	EnemyTeamInfo.push_back(Info);
	SpawnedAI->InitCharacter(SharedMeshSpearmanEnemyTroop, SharedMeshSpear, SharedSpearmanAnimBlueprint, SharedSpearMontageFullbody, SharedSpearMontageUpperbody, &EnemyTeamInfo.back(), &CharacterStateManager, InIsPlayerTeam);
	EnemyTroopTeam[InTroopTeam].push_back(&EnemyTeamInfo.back());
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

void AMBBattleGameMode::UpdateTroopTeamCenter()
{
	for (int i = 0; i < PlayerTroopTeam.size(); ++i)
	{
		float MaxX = 0.f;
		float MinX = 0.f;
		float MaxY = 0.f;
		float MinY = 0.f;

		bool NeedInit = true;

		if (0 == PlayerTroopTeam[i].size())
			continue;
		for (auto it = PlayerTroopTeam[i].begin(); it != PlayerTroopTeam[i].end(); ++it)
		{
			if (nullptr == (*it)->InfoSelfData)
			{
				PlayerTroopTeam[i].erase(it);
				continue;
			}

			float X = (*it)->InfoLocation.X;
			float Y = (*it)->InfoLocation.Y;
			if (NeedInit)
			{
				MaxX = X;
				MinX = X;
				MaxY = Y;
				MinY = Y;
				
				NeedInit = false;
			}

			if (X > MaxX)
				MaxX = X;
			if (X < MinX)
				MinX = X;
			if (Y > MaxY)
				MaxY = Y;
			if (Y < MinY)
				MinY = Y;
		}

		PlayerTroopTeamLocation[i] = FVector((MaxX + MinX) / 2, (MaxY + MinY) / 2, 0.f);
#ifdef DebugMode
		ShowTroopTeamRectangle(MaxX, MinX, MaxY, MinY, FColor::Blue);
#endif // DebugMode
	}

	for (int i = 0; i < EnemyTroopTeam.size(); ++i)
	{
		float MaxX = 0.f;
		float MinX = 0.f;
		float MaxY = 0.f;
		float MinY = 0.f;

		bool NeedInit = true;

		if (0 == EnemyTroopTeam[i].size())
			continue;
		for (auto it = EnemyTroopTeam[i].begin(); it != EnemyTroopTeam[i].end(); ++it)
		{
			if (nullptr == (*it)->InfoSelfData)
			{
				EnemyTroopTeam[i].erase(it);
				continue;
			}

			float X = (*it)->InfoLocation.X;
			float Y = (*it)->InfoLocation.Y;
			if (NeedInit)
			{
				MaxX = X;
				MinX = X;
				MaxY = Y;
				MinY = Y;

				NeedInit = false;
			}

			if (X > MaxX)
				MaxX = X;
			if (X < MinX)
				MinX = X;
			if (Y > MaxY)
				MaxY = Y;
			if (Y < MinY)
				MinY = Y;
		}

		EnemyTroopTeamLocation[i] = FVector((MaxX + MinX) / 2, (MaxY + MinY) / 2, 0.f);
#ifdef DebugMode
		ShowTroopTeamRectangle(MaxX, MinX, MaxY, MinY, FColor::Red);
#endif // DebugMode
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

void AMBBattleGameMode::ShowTeam()
{
	for (auto& Data : PlayerTroopTeam[0])
	{
		DrawDebug::DrawSphere(GetWorld(), Data->InfoLocation);
	}

	for (auto& Data : EnemyTroopTeam[0])
	{
		DrawDebug::DrawSphere(GetWorld(), Data->InfoLocation);
	}
}

void AMBBattleGameMode::ShowTroopTeamCenterLocation()
{
	for (auto& Data : PlayerTroopTeamLocation)
	{
		DrawDebug::DrawSphere(GetWorld(), Data + FVector(0.f, 0.f, 300.f), FColor::Blue);
	}

	for (auto& Data : EnemyTroopTeamLocation)
	{
		DrawDebug::DrawSphere(GetWorld(), Data + FVector(0.f, 0.f, 300.f), FColor::Red);
	}
}

void AMBBattleGameMode::ShowTroopTeamRectangle(float InMaxX, float InMinX, float InMaxY, float InMinY, FColor InColor)
{
	float X[4] = { InMaxX, InMaxX, InMinX, InMinX };
	float Y[4] = { InMaxY ,InMinY, InMinY, InMaxY };

	for (int i = 0; i < 4; ++i)
	{
		int j = i + 1;
		if (j == 4)
			j = 0;

		FVector StartLine = FVector(X[i], Y[i], 0.f);
		FVector EndLine = FVector(X[j], Y[j], 0.f);
		DrawDebug::DrawLine(GetWorld(), StartLine, EndLine, InColor);
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
	StartLocation.Y -= FormationSpace * (Column / 2);

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
