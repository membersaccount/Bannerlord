#include "AI/MBMoveChase.h"
#include "Characters/MBAIBaseCharacter.h"

void MBMoveChase::DecideMove(AMBAIBaseCharacter* InAICharacter)
{
	InAICharacter->AIState.AttitudeData->PassDecideMoveAttitude(InAICharacter);
}

void MBMoveChase::ExecuteMove(AMBAIBaseCharacter* InAICharacter)
{
	InAICharacter->MoveTargetLocation(0.31f);
}
