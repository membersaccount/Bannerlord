#pragma once
#include "AI/MBAttitude.h"

class P1_API MBAttitudeDefensive : public MBAttitude
{
public:
	virtual ~MBAttitudeDefensive() {}

	virtual void DecideAttitude(AMBAIBaseCharacter* InAICharacter) override;

public: // Move -> Attitude
	virtual void PassDecideMoveAttitude(AMBAIBaseCharacter* InAICharacter) override;
};
