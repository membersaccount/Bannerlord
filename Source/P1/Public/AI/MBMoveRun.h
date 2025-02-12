#pragma once
#include "AI/MBMove.h"

class P1_API MBMoveRun : public MBMove
{
public:
	virtual ~MBMoveRun() {}

	virtual void DecideMove(AMBAIBaseCharacter* InAICharacter) override;
	virtual void ExecuteMove(AMBAIBaseCharacter* InAICharacter) override;
};
