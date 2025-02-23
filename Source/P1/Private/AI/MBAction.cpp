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
		// Defend 0.4 ~ 1.0
		InAICharacter->PlayMontageDefend();
		InAICharacter->SetActionDefendTimer(1.5f, 0.4f, 1.f);
		InAICharacter->AIState.ActionData = &InAICharacter->StateManager->ManagerActionDefending;
		return;
	}

	if (50 < FMath::RandRange(1, 100))
	{
		InAICharacter->PlayMontageDefend();
		InAICharacter->SetActionDefendTimer(1.5f, 0.4f, 1.f);
		InAICharacter->AIState.ActionData = &InAICharacter->StateManager->ManagerActionDefending;
		return;
	}

	// Down 2.3, Up 1.8, Right 1.3, Down 0.8, Up 0.54, Right 0.47
	int AttackType = FMath::RandRange(1, 3);
	switch (AttackType)
	{
	case 1:
		InAICharacter->PlayMontageAttack(AttackType);
		InAICharacter->SetActionAttackTimer(2.3f, 0.8f);
		break;
	case 2:
		InAICharacter->PlayMontageAttack(AttackType);
		InAICharacter->SetActionAttackTimer(1.8f, 0.54f);
		break;
	case 3:
		InAICharacter->PlayMontageAttack(AttackType);
		InAICharacter->SetActionAttackTimer(1.3f, 0.47f);
		break;
	default:
		check(false);
	}

	InAICharacter->AIState.ActionData = &InAICharacter->StateManager->ManagerActionAttacking;
}
