#include "AI/MBAttitudeDefensive.h"
#include "Characters/MBAIBaseCharacter.h"
#include "AI/MBStateManager.h"

void MBAttitudeDefensive::DecideAttitude(AMBAIBaseCharacter* const InAICharacter) const
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

void MBAttitudeDefensive::PassDecideMoveAttitude(AMBAIBaseCharacter* const InAICharacter) const
{
	typedef Enums::AI::States::Distance Distance;

	switch (InAICharacter->TargetDistance)
	{
	case Distance::None:
		InAICharacter->AIState.MoveData = &InAICharacter->StateManager->ManagerMoveStop;
		break;
	case Distance::Long:
		InAICharacter->AIState.MoveData = &InAICharacter->StateManager->ManagerMoveCharge;
		break;
	case Distance::Short:
		InAICharacter->ClearTimer(&InAICharacter->RandomLeadTimer);
		InAICharacter->AIState.MoveData = &InAICharacter->StateManager->ManagerMoveCharge;
		break;
	case Distance::Combat:
		InAICharacter->ClearTimer(&InAICharacter->RandomLeadTimer);
		InAICharacter->AIState.MoveData = &InAICharacter->StateManager->ManagerMoveAvoid;
		break;

	case Distance::TooClose:
		InAICharacter->ClearTimer(&InAICharacter->RandomLeadTimer);
		InAICharacter->AIState.MoveData = &InAICharacter->StateManager->ManagerMoveAvoid;
		break;
	}
}
