#pragma once
#include "AI/MBMove.h"

class P1_API MBMoveChase : public MBMove
{
public:
	virtual ~MBMoveChase() {}

	virtual void DecideMove(AMBAIBaseCharacter* const InAICharacter) const override;
	virtual void ExecuteMove(AMBAIBaseCharacter* const InAICharacter) const override;
};
