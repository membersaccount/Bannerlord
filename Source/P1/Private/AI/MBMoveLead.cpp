#include "AI/MBMoveLead.h"
#include "Characters/MBAIBaseCharacter.h"

void MBMoveLead::DecideMove(AMBAIBaseCharacter* const InAICharacter) const
{
	InAICharacter->AIState.AttitudeData->PassDecideMoveAttitude(InAICharacter);
}

void MBMoveLead::ExecuteMove(AMBAIBaseCharacter* const InAICharacter) const
{
	InAICharacter->MoveSideways(0.33f);
}
