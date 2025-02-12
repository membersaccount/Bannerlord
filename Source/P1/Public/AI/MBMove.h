#pragma once

class AMBAIBaseCharacter;

class P1_API MBMove
{
public:
	virtual ~MBMove() = default;

	virtual void DecideMove(AMBAIBaseCharacter* InAICharacter) = 0;
	virtual void ExecuteMove(AMBAIBaseCharacter* InAICharacter) = 0;
};
