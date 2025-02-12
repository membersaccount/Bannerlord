#pragma once
#include "AI/MBMove.h"

class P1_API MBMoveAvoid : public MBMove
{
public:
	virtual ~MBMoveAvoid() {}

	virtual void DecideMove(AMBAIBaseCharacter* InAICharacter) override;
	virtual void ExecuteMove(AMBAIBaseCharacter* InAICharacter) override;
};
