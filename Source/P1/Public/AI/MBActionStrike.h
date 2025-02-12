#pragma once
#include "AI/MBAction.h"

class P1_API MBActionStrike : public MBAction
{
public:
	virtual ~MBActionStrike() {}

	virtual void DecideAction(AMBAIBaseCharacter* InAICharacter) override;
	virtual void ExecuteAction(AMBAIBaseCharacter* InAICharacter) override;
};
