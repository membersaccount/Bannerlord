#include "AI/MBOrderForceStop.h"
#include "Characters/MBAIBaseCharacter.h"
#include "AI/MBStateManager.h"
#include "MBDebug.h"

void MBOrderForceStop::InitOrder(AMBAIBaseCharacter* const InAICharacter) const
{
	InAICharacter->IsIdle = false;
	InAICharacter->AIState.AttitudeData = &InAICharacter->StateManager->ManagerAttitudeIdle;
	InAICharacter->AIState.ActionData = &InAICharacter->StateManager->ManagerActionNone;
	InAICharacter->AIState.MoveData = &InAICharacter->StateManager->ManagerMoveStop;

#ifdef DebugMode
	InAICharacter->AIAttitude = Enums::AI::States::Attitude::Idle;
	Debug::Print("Order ForceStop Called");
#endif // DebugMode
}

void MBOrderForceStop::HandleOrder(AMBAIBaseCharacter* const InAICharacter) const
{
}
