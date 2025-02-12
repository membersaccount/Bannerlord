#include "AI/MBMoveForce.h"
#include "Characters/MBAIBaseCharacter.h"

void MBMoveForce::DecideMove(AMBAIBaseCharacter* InAICharacter)
{
}

void MBMoveForce::ExecuteMove(AMBAIBaseCharacter* InAICharacter)
{
	InAICharacter->MoveForceLocation(0.5f);
}
