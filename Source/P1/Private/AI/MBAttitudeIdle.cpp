#include "AI/MBAttitudeIdle.h"
#include "Characters/MBAIBaseCharacter.h"
#include "AI/MBStateManager.h"

void MBAttitudeIdle::DecideAttitude(AMBAIBaseCharacter* const InAICharacter) const
{
	if (false == InAICharacter->IsTargetExist)
	{
		InAICharacter->AIState.AttitudeData = &InAICharacter->StateManager->ManagerAttitudeIdle;
#ifdef DebugMode
		InAICharacter->AIAttitude = Enums::AI::States::Attitude::Idle;
#endif // DebugMode
		return;
	}

	if (InAICharacter->AIInfo->InfoTargetData->AIState.AttitudeData != &InAICharacter->StateManager->ManagerAttitudeAggressive)
	{
		InAICharacter->AIState.AttitudeData = &InAICharacter->StateManager->ManagerAttitudeAggressive;
#ifdef DebugMode
		InAICharacter->AIAttitude = Enums::AI::States::Attitude::Aggressive;
#endif // DebugMode
		return;
	}

	if (InAICharacter->HP < InAICharacter->AIInfo->InfoTargetData->HP)
	{
		InAICharacter->AIState.AttitudeData = &InAICharacter->StateManager->ManagerAttitudeDefensive;
#ifdef DebugMode
		InAICharacter->AIAttitude = Enums::AI::States::Attitude::Defensive;
#endif // DebugMode
		return;
	}

	InAICharacter->AIState.AttitudeData = &InAICharacter->StateManager->ManagerAttitudeAggressive;
#ifdef DebugMode
	InAICharacter->AIAttitude = Enums::AI::States::Attitude::Aggressive;
#endif // DebugMode
}

void MBAttitudeIdle::PassDecideMoveAttitude(AMBAIBaseCharacter* const InAICharacter) const
{
	return;
}
