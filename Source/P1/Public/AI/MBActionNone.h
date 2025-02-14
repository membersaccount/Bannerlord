#pragma once
#include "AI/MBAction.h"

class P1_API MBActionNone : public MBAction
{
public:
	virtual ~MBActionNone() {}

	virtual void DecideAction(AMBAIBaseCharacter* const InAICharacter) const override;
	virtual void ExecuteAction(AMBAIBaseCharacter* const InAICharacter) const override;
};
