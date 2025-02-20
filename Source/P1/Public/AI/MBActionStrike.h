#pragma once
#include "AI/MBAction.h"

class P1_API MBActionStrike : public MBAction
{
	typedef Enums::AI::States::Distance Distance;

public:
	virtual ~MBActionStrike() {}

	virtual void DecideAction(AMBAIBaseCharacter* const InAICharacter) const override;
	virtual void ExecuteAction(AMBAIBaseCharacter* const InAICharacter) const override;
};
