#include "AI/MBMoveAvoid.h"
#include "Characters/MBAIBaseCharacter.h"

void MBMoveAvoid::DecideMove(AMBAIBaseCharacter* InAICharacter)
{
	InAICharacter->AIState.AttitudeData->PassDecideMoveAttitude(InAICharacter);
}

void MBMoveAvoid::ExecuteMove(AMBAIBaseCharacter* InAICharacter)
{
	InAICharacter->MoveSideways(-0.33f);
}
