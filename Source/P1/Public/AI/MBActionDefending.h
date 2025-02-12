#pragma once
#include "AI/MBAction.h"

class P1_API MBActionDefending : public MBAction
{
public:
	virtual ~MBActionDefending() {}

	virtual void DecideAction(AMBAIBaseCharacter* InAICharacter) override;
	virtual void ExecuteAction(AMBAIBaseCharacter* InAICharacter) override;
};
