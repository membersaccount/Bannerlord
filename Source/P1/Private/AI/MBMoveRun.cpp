#include "AI/MBMoveRun.h"
#include "Characters/MBAIBaseCharacter.h"

void MBMoveRun::DecideMove(AMBAIBaseCharacter* InAICharacter)
{
}

void MBMoveRun::ExecuteMove(AMBAIBaseCharacter* InAICharacter)
{
	InAICharacter->MoveTargetLocation(0.4f);
}
