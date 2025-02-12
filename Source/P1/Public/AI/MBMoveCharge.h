#pragma once
#include "AI/MBMove.h"

class MBAttitude;

class P1_API MBMoveCharge : public MBMove
{
public:
	virtual ~MBMoveCharge() {}

	virtual void DecideMove(AMBAIBaseCharacter* InAICharacter) override;
	virtual void ExecuteMove(AMBAIBaseCharacter* InAICharacter) override;
};
