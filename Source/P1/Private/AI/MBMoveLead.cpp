#include "AI/MBMoveLead.h"
#include "Characters/MBAIBaseCharacter.h"

void MBMoveLead::DecideMove(AMBAIBaseCharacter* InAICharacter)
{
	InAICharacter->AIState.AttitudeData->PassDecideMoveAttitude(InAICharacter);
}

void MBMoveLead::ExecuteMove(AMBAIBaseCharacter* InAICharacter)
{
	InAICharacter->MoveTargetLocation(0.33f);
}
