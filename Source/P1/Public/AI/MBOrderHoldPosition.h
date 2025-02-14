#pragma once
#include "AI/MBOrder.h"

class P1_API MBOrderHoldPosition : public MBOrder
{
public:
	virtual ~MBOrderHoldPosition() {}

	virtual void InItOrder(AMBAIBaseCharacter* const InAICharacter) const override;
	virtual void HandleOrder(AMBAIBaseCharacter* const InAICharacter) const override;
};
