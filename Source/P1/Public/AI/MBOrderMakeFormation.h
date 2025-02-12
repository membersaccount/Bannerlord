#pragma once
#include "AI/MBOrder.h"

class P1_API MBOrderMakeFormation : public MBOrder
{
public:
	virtual ~MBOrderMakeFormation() {}

	virtual void InItOrder(AMBAIBaseCharacter* InAICharacter) override;
	virtual void HandleOrder(AMBAIBaseCharacter* InAICharacter) override;
};
