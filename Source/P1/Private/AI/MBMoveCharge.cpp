#include "AI/MBMoveCharge.h"
#include "Characters/MBAIBaseCharacter.h"
#include "AI/MBStateManager.h"

void MBMoveCharge::DecideMove(AMBAIBaseCharacter* InAICharacter)
{
	InAICharacter->AIState.AttitudeData->PassDecideMoveAttitude(InAICharacter);
}

void MBMoveCharge::ExecuteMove(AMBAIBaseCharacter* InAICharacter)
{
	InAICharacter->MoveTargetLocation(0.45f);
}
