#pragma once
#include "AI/MBMove.h"

class P1_API MBMoveLead : public MBMove
{
public:
	virtual ~MBMoveLead() {}

	virtual void DecideMove(AMBAIBaseCharacter* const InAICharacter) const override;
	virtual void ExecuteMove(AMBAIBaseCharacter* const InAICharacter) const override;
};
