#pragma once
#include "AI/MBOrder.h"

class P1_API MBOrderEngageBattle : public MBOrder
{
public:
	virtual ~MBOrderEngageBattle() {}

	virtual void InitOrder(AMBAIBaseCharacter* const InAICharacter) const override;
	virtual void HandleOrder(AMBAIBaseCharacter* const InAICharacter) const override;
};
