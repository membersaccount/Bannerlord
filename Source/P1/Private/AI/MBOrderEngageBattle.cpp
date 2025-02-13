#include "AI/MBOrderEngageBattle.h"
#include "Characters/MBAIBaseCharacter.h"

void MBOrderEngageBattle::InItOrder(AMBAIBaseCharacter* const InAICharacter) const
{
}

void MBOrderEngageBattle::HandleOrder(AMBAIBaseCharacter* const InAICharacter) const
{
	InAICharacter->CheckTargetExist();
	if (false == InAICharacter->IsTargetExist)
		return;

	InAICharacter->CalculateDistance(InAICharacter->AIInfo->InfoTargetData->AIInfo->InfoLocation);
	InAICharacter->DecideTargetDistance();

	InAICharacter->AIState.AttitudeData->DecideAttitude(InAICharacter);
	InAICharacter->AIState.ActionData->DecideAction(InAICharacter);
	InAICharacter->AIState.MoveData->DecideMove(InAICharacter);
	InAICharacter->AIState.ActionData->ExecuteAction(InAICharacter);
	InAICharacter->AIState.MoveData->ExecuteMove(InAICharacter);
}
