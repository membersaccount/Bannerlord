#include "AI/MBMoveStop.h"
#include "Characters/MBAIBaseCharacter.h"

void MBMoveStop::DecideMove(AMBAIBaseCharacter* const InAICharacter) const
{
	InAICharacter->AIState.AttitudeData->PassDecideMoveAttitude(InAICharacter);
}

void MBMoveStop::ExecuteMove(AMBAIBaseCharacter* const InAICharacter) const
{
}
