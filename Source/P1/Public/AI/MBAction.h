#pragma once

class AMBAIBaseCharacter;

class P1_API MBAction
{
public:
	virtual ~MBAction() = default;

	virtual void DecideAction(AMBAIBaseCharacter* InAICharacter) = 0;
	virtual void ExecuteAction(AMBAIBaseCharacter* InAICharacter) = 0;
};
