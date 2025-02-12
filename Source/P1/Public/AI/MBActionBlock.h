#pragma once
#include "AI/MBAction.h"

class P1_API MBActionBlock : public MBAction
{
public:
	virtual ~MBActionBlock() {}

	virtual void DecideAction(AMBAIBaseCharacter* InAICharacter) override;
	virtual void ExecuteAction(AMBAIBaseCharacter* InAICharacter) override;
};
