#pragma once
#include "AI/MBMove.h"

class MBAttitude;

class P1_API MBMoveCharge : public MBMove
{
public:
	virtual ~MBMoveCharge() {}

	virtual void DecideMove(AMBAIBaseCharacter* const InAICharacter) const override;
	virtual void ExecuteMove(AMBAIBaseCharacter* const InAICharacter) const override;
};
