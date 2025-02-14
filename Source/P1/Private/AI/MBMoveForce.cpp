#include "AI/MBMoveForce.h"
#include "Characters/MBAIBaseCharacter.h"

void MBMoveForce::DecideMove(AMBAIBaseCharacter* const InAICharacter) const
{
}

void MBMoveForce::ExecuteMove(AMBAIBaseCharacter* const InAICharacter) const
{
	InAICharacter->MoveForceLocation(0.45f);
}
