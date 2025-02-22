#pragma once
#include "AI/MBOrder.h"

class P1_API MBOrderMoveLocation : public MBOrder
{
public:
	virtual ~MBOrderMoveLocation() {}

	virtual void InitOrder(AMBAIBaseCharacter* const InAICharacter) const override;
	virtual void HandleOrder(AMBAIBaseCharacter* const InAICharacter) const override;
};
