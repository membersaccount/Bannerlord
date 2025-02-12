#include "AI/MBMoveStop.h"
#include "Characters/MBAIBaseCharacter.h"

void MBMoveStop::DecideMove(AMBAIBaseCharacter* InAICharacter)
{
	InAICharacter->AIState.AttitudeData->PassDecideMoveAttitude(InAICharacter);
}

void MBMoveStop::ExecuteMove(AMBAIBaseCharacter* InAICharacter)
{
}
