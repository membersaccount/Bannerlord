#include "AI/MBAttitudeIdle.h"
#include "Characters/MBAIBaseCharacter.h"
#include "AI/MBStateManager.h"

void MBAttitudeIdle::DecideAttitude(AMBAIBaseCharacter* const InAICharacter) const
{
	if (false == InAICharacter->IsTargetExist)
	{
		InAICharacter->AIState.AttitudeData = &InAICharacter->StateManager->ManagerAttitudeIdle;
		return;
	}

	if (InAICharacter->AIState.AttitudeData != &InAICharacter->StateManager->ManagerAttitudeAggressive)
	{
		InAICharacter->AIState.AttitudeData = &InAICharacter->StateManager->ManagerAttitudeAggressive;
		return;
	}

	if (InAICharacter->HP < InAICharacter->AIInfo->InfoTargetData->HP)
	{
		InAICharacter->AIState.AttitudeData = &InAICharacter->StateManager->ManagerAttitudeDefensive;
		return;
	}

	InAICharacter->AIState.AttitudeData = &InAICharacter->StateManager->ManagerAttitudeAggressive;
}

void MBAttitudeIdle::PassDecideMoveAttitude(AMBAIBaseCharacter* const InAICharacter) const
{
	return;
}
