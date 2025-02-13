#include "AI/MBMoveRun.h"
#include "Characters/MBAIBaseCharacter.h"

void MBMoveRun::DecideMove(AMBAIBaseCharacter* const InAICharacter) const
{
}

void MBMoveRun::ExecuteMove(AMBAIBaseCharacter* const InAICharacter) const
{
	InAICharacter->MoveTargetLocation(0.4f);
}
