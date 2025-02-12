#pragma once

class AMBAIBaseCharacter;

class P1_API MBOrder
{
public:
	virtual ~MBOrder() = default;

	virtual void InItOrder(AMBAIBaseCharacter* InAICharacter) = 0;
	virtual void HandleOrder(AMBAIBaseCharacter* InAICharacter) = 0;
};
