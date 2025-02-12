#pragma once
#include "AI/MBAction.h"

class P1_API MBActionAttacking : public MBAction
{
public:
	virtual ~MBActionAttacking() {}

	virtual void DecideAction(AMBAIBaseCharacter* InAICharacter) override;
	virtual void ExecuteAction(AMBAIBaseCharacter* InAICharacter) override;
};
