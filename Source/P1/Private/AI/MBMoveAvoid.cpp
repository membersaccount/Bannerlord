#include "AI/MBMoveAvoid.h"
#include "Characters/MBAIBaseCharacter.h"

void MBMoveAvoid::DecideMove(AMBAIBaseCharacter* InAICharacter) const
{
	InAICharacter->AIState.AttitudeData->PassDecideMoveAttitude(InAICharacter);
}

void MBMoveAvoid::ExecuteMove(AMBAIBaseCharacter* InAICharacter) const
{
	InAICharacter->MoveTargetLocation(-0.3f);
}
