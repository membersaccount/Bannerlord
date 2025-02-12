#pragma once
#include "AI/MBAction.h"

class P1_API MBActionNone : public MBAction
{
public:
	virtual ~MBActionNone() {}

	virtual void DecideAction(AMBAIBaseCharacter* InAICharacter) override;
	virtual void ExecuteAction(AMBAIBaseCharacter* InAICharacter) override;
};
