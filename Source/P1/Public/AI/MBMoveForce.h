#pragma once
#include "AI/MBMove.h"

class P1_API MBMoveForce : public MBMove
{
public:
	virtual ~MBMoveForce() {}

	virtual void DecideMove(AMBAIBaseCharacter* InAICharacter) override;
	virtual void ExecuteMove(AMBAIBaseCharacter* InAICharacter) override;

};
