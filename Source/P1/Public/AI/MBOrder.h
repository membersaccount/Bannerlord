#pragma once

class AMBAIBaseCharacter;

class P1_API MBOrder
{
public:
	virtual ~MBOrder() = default;

	virtual void InitOrder(AMBAIBaseCharacter* const InAICharacter) const = 0;
	virtual void HandleOrder(AMBAIBaseCharacter* const InAICharacter) const = 0;
};
