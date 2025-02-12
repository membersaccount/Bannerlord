#pragma once
#include "AI/MBMove.h"

class P1_API MBMoveLead : public MBMove
{
public:
	virtual ~MBMoveLead() {}

	virtual void DecideMove(AMBAIBaseCharacter* InAICharacter) override;
	virtual void ExecuteMove(AMBAIBaseCharacter* InAICharacter) override;
};
