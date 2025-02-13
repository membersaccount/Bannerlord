#pragma once
#include "AI/MBAttitude.h"

class P1_API MBAttitudeDefensive : public MBAttitude
{
public:
	virtual ~MBAttitudeDefensive() {}

	virtual void DecideAttitude(AMBAIBaseCharacter* const InAICharacter) const override;

public: // Move -> Attitude
	virtual void PassDecideMoveAttitude(AMBAIBaseCharacter* const InAICharacter) const override;
};
