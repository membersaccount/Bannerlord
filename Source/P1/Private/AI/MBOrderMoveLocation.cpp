#include "AI/MBOrderMoveLocation.h"
#include "Characters/MBAIBaseCharacter.h"
#include "AI/MBStateManager.h"

void MBOrderMoveLocation::InitOrder(AMBAIBaseCharacter* const InAICharacter) const
{
	InAICharacter->IsIdle = false;
	InAICharacter->IsArrivedForceLocation = false;
	InAICharacter->AIState.AttitudeData = &InAICharacter->StateManager->ManagerAttitudeIdle;
	InAICharacter->AIState.MoveData = &InAICharacter->StateManager->ManagerMoveForce;
}

void MBOrderMoveLocation::HandleOrder(AMBAIBaseCharacter* const InAICharacter) const
{
	if (true == InAICharacter->IsArrivedForceLocation)
	{
		InAICharacter->TurnToTarget();
		InAICharacter->AIState.MoveData = &InAICharacter->StateManager->ManagerMoveStop;
		InAICharacter->AIState.ActionData->DecideAction(InAICharacter);
		InAICharacter->AIState.ActionData->ExecuteAction(InAICharacter);
		return;
	}

	InAICharacter->CheckForceLocationArrive();
	InAICharacter->AIState.ActionData->DecideAction(InAICharacter);
	InAICharacter->AIState.ActionData->ExecuteAction(InAICharacter);
	InAICharacter->AIState.MoveData->ExecuteMove(InAICharacter);
}
