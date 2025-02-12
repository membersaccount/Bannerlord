#include "AI/MBMoveAvoid.h"
#include "Characters/MBAIBaseCharacter.h"

void MBMoveAvoid::DecideMove(AMBAIBaseCharacter* InAICharacter)
{
	InAICharacter->AIState.AttitudeData->PassDecideMoveAttitude(InAICharacter);
}

void MBMoveAvoid::ExecuteMove(AMBAIBaseCharacter* InAICharacter)
{
	InAICharacter->MoveTargetLocation(-0.30f);
}
