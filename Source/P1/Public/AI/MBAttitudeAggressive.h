#pragma once
#include "AI/MBAttitude.h"

class P1_API MBAttitudeAggressive : public MBAttitude
{
public:
	virtual ~MBAttitudeAggressive() {}

	virtual void DecideAttitude(AMBAIBaseCharacter* InAICharacter) override;

public: // Move -> Attitude => Only EngageBattle
	virtual void PassDecideMoveAttitude(AMBAIBaseCharacter* InAICharacter) override;
};
