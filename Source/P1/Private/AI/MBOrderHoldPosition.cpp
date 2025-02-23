#include "AI/MBOrderHoldPosition.h"
#include "Characters/MBAIBaseCharacter.h"
#include "AI/MBStateManager.h"

void MBOrderHoldPosition::InitOrder(AMBAIBaseCharacter* const InAICharacter) const
{
	InAICharacter->IsIdle = false;
	InAICharacter->AIState.AttitudeData = &InAICharacter->StateManager->ManagerAttitudeIdle;
	InAICharacter->AIState.MoveData = &InAICharacter->StateManager->ManagerMoveStop;

#ifdef DebugMode
	InAICharacter->AIAttitude = Enums::AI::States::Attitude::Idle;
#endif // DebugMode
}

void MBOrderHoldPosition::HandleOrder(AMBAIBaseCharacter* const InAICharacter) const
{
	InAICharacter->AIState.ActionData->DecideAction(InAICharacter);
	InAICharacter->AIState.ActionData->ExecuteAction(InAICharacter);
}
