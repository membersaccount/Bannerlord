#include "AI/MBOrderEngageBattle.h"
#include "Characters/MBAIBaseCharacter.h"
#include "GameModes/MBBattleGameMode.h"
#include "MBDebug.h"

void MBOrderEngageBattle::InitOrder(AMBAIBaseCharacter* const InAICharacter) const
{
	InAICharacter->IsIdle = false;
}

void MBOrderEngageBattle::HandleOrder(AMBAIBaseCharacter* const InAICharacter) const
{
	//InAICharacter->CheckTargetExist();
	//if (false == InAICharacter->IsTargetExist)
	//{
	//	AMBBattleGameMode* GameMode = Cast<AMBBattleGameMode>(InAICharacter->CachedWorld->GetAuthGameMode());

	//	int TargetTeamIndex = GameMode->TargetSearchCloseTeam(InAICharacter);
	//	GameMode->SearchCloseTarget(InAICharacter, TargetTeamIndex);

	//	InAICharacter->CheckTargetExist();
	//	if (false == InAICharacter->IsTargetExist)
	//	{
	//		return;
	//	}
	//}

	if (nullptr == InAICharacter->AIInfo->InfoTargetData || InAICharacter->AIInfo->InfoTargetData->GetIsDead())
	{
		return;
	}

	InAICharacter->CalculateDistance(InAICharacter->AIInfo->InfoTargetData->AIInfo->InfoLocation);
	InAICharacter->DecideTargetDistance();

	InAICharacter->AIState.AttitudeData->DecideAttitude(InAICharacter);
	InAICharacter->AIState.ActionData->DecideAction(InAICharacter);
	InAICharacter->AIState.MoveData->DecideMove(InAICharacter);
	InAICharacter->AIState.ActionData->ExecuteAction(InAICharacter);
	InAICharacter->AIState.MoveData->ExecuteMove(InAICharacter);
}
