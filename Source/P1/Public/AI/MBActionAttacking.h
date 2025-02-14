#pragma once
#include "AI/MBAction.h"

class P1_API MBActionAttacking : public MBAction
{
public:
	virtual ~MBActionAttacking() {}

	virtual void DecideAction(AMBAIBaseCharacter* const InAICharacter) const override;
	virtual void ExecuteAction(AMBAIBaseCharacter* const InAICharacter) const override;
};
