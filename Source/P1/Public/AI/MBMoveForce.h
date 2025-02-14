#pragma once
#include "AI/MBMove.h"

class P1_API MBMoveForce : public MBMove
{
public:
	virtual ~MBMoveForce() {}

	virtual void DecideMove(AMBAIBaseCharacter* const InAICharacter) const override;
	virtual void ExecuteMove(AMBAIBaseCharacter* const InAICharacter) const override;

};
