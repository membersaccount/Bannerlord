#pragma once
#include "AI/MBOrder.h"

class P1_API MBOrderMakeFormation : public MBOrder
{
public:
	virtual ~MBOrderMakeFormation() {}

	virtual void InItOrder(AMBAIBaseCharacter* const InAICharacter) const override;
	virtual void HandleOrder(AMBAIBaseCharacter* const InAICharacter) const override;
};
