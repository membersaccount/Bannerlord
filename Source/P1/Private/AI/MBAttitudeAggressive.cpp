#include "AI/MBAttitudeAggressive.h"
#include "Characters/MBAIBaseCharacter.h"
#include "AI/MBStateManager.h"
#include "Datas/MBEnums.h"

void MBAttitudeAggressive::DecideAttitude(AMBAIBaseCharacter* const InAICharacter) const
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

void MBAttitudeAggressive::PassDecideMoveAttitude(AMBAIBaseCharacter* const InAICharacter) const
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
		if (InAICharacter->AIInfo->InfoTargetData->AIState.MoveData == &InAICharacter->StateManager->ManagerMoveLead)
		{
			InAICharacter->ClearTimer(&InAICharacter->RandomLeadTimer);
		}

		InAICharacter->AIState.MoveData = &InAICharacter->StateManager->ManagerMoveCharge;
		break;
	case Distance::Combat:
		if (InAICharacter->AIInfo->InfoTargetData->AIState.MoveData == &InAICharacter->StateManager->ManagerMoveLead)
		{
			InAICharacter->ClearTimer(&InAICharacter->RandomLeadTimer);
			InAICharacter->AIState.MoveData = &InAICharacter->StateManager->ManagerMoveChase;
			break;
		}

		if (InAICharacter->AIInfo->InfoTargetData->TargetDistance == Distance::Combat)
		{
			if (false == InAICharacter->IsTimerActive(&InAICharacter->RandomLeadTimer))
			{
				InAICharacter->AIState.MoveData = &InAICharacter->StateManager->ManagerMoveLead;
				InAICharacter->SetLeadTimer(FMath::RandRange(2.5f, 7.5f));
			}
			break;
		}

		InAICharacter->AIState.MoveData = &InAICharacter->StateManager->ManagerMoveChase;
		break;

	case Distance::TooClose:
		if (InAICharacter->AIState.MoveData == &InAICharacter->StateManager->ManagerMoveLead)
		{
			break;
		}

		if (InAICharacter->IsTimerActive(&InAICharacter->RandomLeadTimer))
		{
			InAICharacter->ClearTimer(&InAICharacter->RandomLeadTimer);
		}

		InAICharacter->AIState.MoveData = &InAICharacter->StateManager->ManagerMoveWalk;
		break;
	}
}
