#include "AI/MBOrderMakeFormation.h"
#include "Characters/MBAIBaseCharacter.h"
#include "AI/MBStateManager.h"

void MBOrderMakeFormation::InItOrder(AMBAIBaseCharacter* InAICharacter)
{
	InAICharacter->IsArrivedForceLocation = false;
	InAICharacter->AIState.AttitudeData = &InAICharacter->StateManager->ManagerAttitudeIdle;
	InAICharacter->AIState.MoveData = &InAICharacter->StateManager->ManagerMoveForce;
}

void MBOrderMakeFormation::HandleOrder(AMBAIBaseCharacter* InAICharacter)
{
	if (true == InAICharacter->IsArrivedForceLocation)
	{
		InAICharacter->AIState.MoveData = &InAICharacter->StateManager->ManagerMoveStop;
		InAICharacter->AIState.ActionData->DecideAction(InAICharacter);
		InAICharacter->AIState.ActionData->ExecuteAction(InAICharacter);
		return;
	}

	InAICharacter->AIState.ActionData->DecideAction(InAICharacter);
	InAICharacter->AIState.ActionData->ExecuteAction(InAICharacter);
	InAICharacter->AIState.MoveData->ExecuteMove(InAICharacter);
}
