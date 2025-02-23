#include "AI/MBMoveWalk.h"
#include "Characters/MBAIBaseCharacter.h"

void MBMoveWalk::DecideMove(AMBAIBaseCharacter* const InAICharacter) const
{
	InAICharacter->AIState.AttitudeData->PassDecideMoveAttitude(InAICharacter);
}

void MBMoveWalk::ExecuteMove(AMBAIBaseCharacter* const InAICharacter) const
{
	InAICharacter->MoveTargetLocation(0.2f);
}
