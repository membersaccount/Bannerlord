#pragma once
#include "AI/MBMove.h"

class P1_API MBMoveChase : public MBMove
{
public:
	virtual ~MBMoveChase() {}

	virtual void DecideMove(AMBAIBaseCharacter* InAICharacter) override;
	virtual void ExecuteMove(AMBAIBaseCharacter* InAICharacter) override;
};
