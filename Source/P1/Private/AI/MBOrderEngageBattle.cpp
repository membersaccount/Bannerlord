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
	InAICharacter->CheckTargetExist();
	if (false == InAICharacter->IsTargetExist)
	{
		AMBBattleGameMode* GameMode = Cast<AMBBattleGameMode>(InAICharacter->CachedWorld->GetAuthGameMode());

		GameMode->TargetSearchCloseTeam();

		InAICharacter->CheckTargetExist();
		if (false == InAICharacter->IsTargetExist)
		{
			return;
		}
	}

	InAICharacter->CalculateDistance(InAICharacter->AIInfo->InfoTargetData->AIInfo->InfoLocation);
	InAICharacter->DecideTargetDistance();

	InAICharacter->AIState.AttitudeData->DecideAttitude(InAICharacter);
	InAICharacter->AIState.ActionData->DecideAction(InAICharacter);
	InAICharacter->AIState.MoveData->DecideMove(InAICharacter);
	InAICharacter->AIState.ActionData->ExecuteAction(InAICharacter);
	InAICharacter->AIState.MoveData->ExecuteMove(InAICharacter);
}
