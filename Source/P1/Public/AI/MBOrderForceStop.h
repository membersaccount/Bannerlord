#pragma once
#include "AI/MBOrder.h"

class P1_API MBOrderForceStop : public MBOrder
{
public:
	virtual ~MBOrderForceStop() {}

	virtual void InitOrder(AMBAIBaseCharacter* const InAICharacter) const override;
	virtual void HandleOrder(AMBAIBaseCharacter* const InAICharacter) const override;
};
