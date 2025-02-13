#pragma once
#include "AI/MBAttitude.h"

class P1_API MBAttitudeAggressive : public MBAttitude
{
public:
	virtual ~MBAttitudeAggressive() {}

	virtual void DecideAttitude(AMBAIBaseCharacter* const InAICharacter) const override;

public: // Move -> Attitude => Only EngageBattle
	virtual void PassDecideMoveAttitude(AMBAIBaseCharacter* const InAICharacter) const override;
};
