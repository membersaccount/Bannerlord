#pragma once
#include "AI/MBOrder.h"

class P1_API MBOrderEngageBattle : public MBOrder
{
public:
	virtual ~MBOrderEngageBattle() {}

	virtual void InItOrder(AMBAIBaseCharacter* InAICharacter) override;
	virtual void HandleOrder(AMBAIBaseCharacter* InAICharacter) override;
};
