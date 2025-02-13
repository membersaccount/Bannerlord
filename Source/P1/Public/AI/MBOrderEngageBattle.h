#pragma once
#include "AI/MBOrder.h"

class P1_API MBOrderEngageBattle : public MBOrder
{
public:
	virtual ~MBOrderEngageBattle() {}

	virtual void InItOrder(AMBAIBaseCharacter* const InAICharacter) const override;
	virtual void HandleOrder(AMBAIBaseCharacter* const InAICharacter) const override;
};
