#include "AI/MBAction.h"
#include "Characters/MBAIBaseCharacter.h"
#include "AI/MBStateManager.h"

void MBAction::WideDecideAction(AMBAIBaseCharacter* const InAICharacter) const
{
	if (nullptr == InAICharacter->AIInfo->InfoTargetData &&
		true == InAICharacter->AIInfo->InfoTargetData->GetIsDead())
	{
		//TODO: Search other Target
		return;
	}

	if (true == InAICharacter->EnableActionDelay)
		return;

	if (true == InAICharacter->EnableAttackDelay)
	{
		InAICharacter->SetActionDefendTimer(1.5f, 0.2f, 0.9f);
		InAICharacter->AIState.ActionData = &InAICharacter->StateManager->ManagerActionDefending;
		InAICharacter->PlaySpearAttackMontage(2);
		return;
	}

	if (50 < FMath::RandRange(1, 100))
	{
		InAICharacter->SetActionDefendTimer(1.5f, 0.2f, 0.9f);
		InAICharacter->AIState.ActionData = &InAICharacter->StateManager->ManagerActionDefending;
		InAICharacter->PlaySpearAttackMontage(2);
		return;
	}

	InAICharacter->SetActionAttackTimer(2.3f, 0.65f, 0.65f);
	InAICharacter->AIState.ActionData = &InAICharacter->StateManager->ManagerActionAttacking;
	InAICharacter->PlaySpearAttackMontage(1);
}
