#include "AI/MBMoveChase.h"
#include "Characters/MBAIBaseCharacter.h"

void MBMoveChase::DecideMove(AMBAIBaseCharacter* const InAICharacter) const
{
	InAICharacter->AIState.AttitudeData->PassDecideMoveAttitude(InAICharacter);
}

void MBMoveChase::ExecuteMove(AMBAIBaseCharacter* const InAICharacter) const
{
	InAICharacter->MoveTargetLocation(0.3f);
}
