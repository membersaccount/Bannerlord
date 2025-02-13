#pragma once
#include "AI/MBAction.h"

class P1_API MBActionDefending : public MBAction
{
public:
	virtual ~MBActionDefending() {}

	virtual void DecideAction(AMBAIBaseCharacter* const InAICharacter) const override;
	virtual void ExecuteAction(AMBAIBaseCharacter* const InAICharacter) const override;
};
