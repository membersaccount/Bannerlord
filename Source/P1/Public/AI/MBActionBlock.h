#pragma once
#include "AI/MBAction.h"

class P1_API MBActionBlock : public MBAction
{
public:
	virtual ~MBActionBlock() {}

	virtual void DecideAction(AMBAIBaseCharacter* const InAICharacter) const override;
	virtual void ExecuteAction(AMBAIBaseCharacter* const InAICharacter) const override;
};
