#include "AI/MBOrderHoldPosition.h"
#include "Characters/MBAIBaseCharacter.h"
#include "AI/MBStateManager.h"

void MBOrderHoldPosition::InItOrder(AMBAIBaseCharacter* const InAICharacter) const
{
	InAICharacter->IsIdle = false;
	InAICharacter->AIState.AttitudeData = &InAICharacter->StateManager->ManagerAttitudeIdle;
	InAICharacter->AIState.MoveData = &InAICharacter->StateManager->ManagerMoveStop;
}

void MBOrderHoldPosition::HandleOrder(AMBAIBaseCharacter* const InAICharacter) const
{
	InAICharacter->AIState.ActionData->DecideAction(InAICharacter);
	InAICharacter->AIState.ActionData->ExecuteAction(InAICharacter);
}
