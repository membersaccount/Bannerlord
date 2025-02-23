#include "AI/MBMoveForce.h"
#include "Characters/MBAIBaseCharacter.h"

void MBMoveForce::DecideMove(AMBAIBaseCharacter* const InAICharacter) const
{
	InAICharacter->AIState.AttitudeData->PassDecideMoveAttitude(InAICharacter);
}

void MBMoveForce::ExecuteMove(AMBAIBaseCharacter* const InAICharacter) const
{
	InAICharacter->MoveForceLocation(0.45f);
}
