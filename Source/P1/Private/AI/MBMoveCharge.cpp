#include "AI/MBMoveCharge.h"
#include "Characters/MBAIBaseCharacter.h"
#include "AI/MBStateManager.h"

void MBMoveCharge::DecideMove(AMBAIBaseCharacter* const InAICharacter) const
{
	InAICharacter->AIState.AttitudeData->PassDecideMoveAttitude(InAICharacter);
}

void MBMoveCharge::ExecuteMove(AMBAIBaseCharacter* const InAICharacter) const
{
	InAICharacter->MoveTargetLocation(0.45f);
}
