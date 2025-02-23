#include "AI/MBActionStrike.h"
#include "Characters/MBAIBaseCharacter.h"
#include "AI/MBStateManager.h"

void MBActionStrike::DecideAction(AMBAIBaseCharacter* const InAICharacter) const
{
}

void MBActionStrike::ExecuteAction(AMBAIBaseCharacter* const InAICharacter) const
{
	if (nullptr == InAICharacter->AIInfo->InfoTargetData)
	{
		InAICharacter->AIState.ActionData = &InAICharacter->StateManager->ManagerActionAttacking;
		return;
	}

	if (InAICharacter->AIInfo->InfoTargetData->AIState.ActionData != &InAICharacter->StateManager->ManagerActionBlock)
	{
		//InAICharacter->AIInfo->InfoTargetData->OnHit(InAICharacter->Damage);
	}

	InAICharacter->AIState.ActionData = &InAICharacter->StateManager->ManagerActionAttacking;
}
