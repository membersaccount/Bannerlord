#pragma once
#include "AI/MBMove.h"

class P1_API MBMoveStop : public MBMove
{
public:
	virtual ~MBMoveStop() {}

	virtual void DecideMove(AMBAIBaseCharacter* const InAICharacter) const override;
	virtual void ExecuteMove(AMBAIBaseCharacter* const InAICharacter) const override;
};
