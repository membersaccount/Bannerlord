#pragma once
#include "AI/MBMove.h"

class P1_API MBMoveWalk : public MBMove
{
public:
	virtual ~MBMoveWalk() {}

	virtual void DecideMove(AMBAIBaseCharacter* const InAICharacter) const override;
	virtual void ExecuteMove(AMBAIBaseCharacter* const InAICharacter) const override;
};
