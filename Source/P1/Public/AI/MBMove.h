#pragma once

class AMBAIBaseCharacter;

class P1_API MBMove
{
public:
	virtual ~MBMove() = default;

	virtual void DecideMove(AMBAIBaseCharacter* const InAICharacter) const = 0;
	virtual void ExecuteMove(AMBAIBaseCharacter* const InAICharacter) const = 0;
};
