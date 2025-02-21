#pragma once
#include "AI/MBOrder.h"

class P1_API MBOrderHoldPosition : public MBOrder
{
public:
	virtual ~MBOrderHoldPosition() {}

	virtual void InitOrder(AMBAIBaseCharacter* const InAICharacter) const override;
	virtual void HandleOrder(AMBAIBaseCharacter* const InAICharacter) const override;
};
