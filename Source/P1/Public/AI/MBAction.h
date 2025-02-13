#pragma once

class AMBAIBaseCharacter;

namespace Enums{ namespace AI{ namespace States
{
	enum class Distance;
} } }

class P1_API MBAction
{
	typedef Enums::AI::States::Distance Distance;
public:
	virtual ~MBAction() = default;

	virtual void DecideAction(AMBAIBaseCharacter* const InAICharacter) const = 0;
	virtual void ExecuteAction(AMBAIBaseCharacter* const InAICharacter) const = 0;


};
