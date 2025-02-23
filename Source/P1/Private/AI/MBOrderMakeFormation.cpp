#include "AI/MBOrderMakeFormation.h"
#include "Characters/MBAIBaseCharacter.h"
#include "AI/MBStateManager.h"

void MBOrderMakeFormation::InitOrder(AMBAIBaseCharacter* const InAICharacter) const
{
	InAICharacter->IsIdle = false;
	InAICharacter->IsArrivedForceLocation = false;
	InAICharacter->AIState.AttitudeData = &InAICharacter->StateManager->ManagerAttitudeIdle;
	InAICharacter->AIState.MoveData = &InAICharacter->StateManager->ManagerMoveForce;

#ifdef DebugMode
	InAICharacter->AIAttitude = Enums::AI::States::Attitude::Idle;
#endif // DebugMode
}

void MBOrderMakeFormation::HandleOrder(AMBAIBaseCharacter* const InAICharacter) const
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

	//-0
	//InAICharacter->AIState.ActionData->DecideAction(InAICharacter);
	//InAICharacter->AIState.ActionData->ExecuteAction(InAICharacter);
}
