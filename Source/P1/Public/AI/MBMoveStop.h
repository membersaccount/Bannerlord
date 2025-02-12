#pragma once
#include "AI/MBMove.h"

class P1_API MBMoveStop : public MBMove
{
public:
	virtual ~MBMoveStop() {}

	virtual void DecideMove(AMBAIBaseCharacter* InAICharacter) override;
	virtual void ExecuteMove(AMBAIBaseCharacter* InAICharacter) override;
};
