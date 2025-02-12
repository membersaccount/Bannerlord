#pragma once
#include "AI/MBOrder.h"

class P1_API MBOrderHoldPosition : public MBOrder
{
public:
	virtual ~MBOrderHoldPosition() {}

	virtual void InItOrder(AMBAIBaseCharacter* InAICharacter) override;
	virtual void HandleOrder(AMBAIBaseCharacter* InAICharacter) override;
};
