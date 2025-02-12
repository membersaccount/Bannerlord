#include "AI/MBAttitudeDefensive.h"
#include "Characters/MBAIBaseCharacter.h"
#include "AI/MBStateManager.h"

void MBAttitudeDefensive::DecideAttitude(AMBAIBaseCharacter* InAICharacter)
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

void MBAttitudeDefensive::PassDecideMoveAttitude(AMBAIBaseCharacter* InAICharacter)
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
		InAICharacter->AIState.MoveData = &InAICharacter->StateManager->ManagerMoveCharge;
		break;
	case Distance::Combat:
		InAICharacter->AIState.MoveData = &InAICharacter->StateManager->ManagerMoveAvoid;
		break;
	case Distance::TooClose:
		InAICharacter->AIState.MoveData = &InAICharacter->StateManager->ManagerMoveAvoid;
		break;
	}
}
