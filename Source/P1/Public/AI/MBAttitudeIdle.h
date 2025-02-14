#pragma once
#include "AI/MBAttitude.h"

class P1_API MBAttitudeIdle : public MBAttitude
{
public:
	virtual ~MBAttitudeIdle() {}

	virtual void DecideAttitude(AMBAIBaseCharacter* const InAICharacter) const override;

public: 
	virtual void PassDecideMoveAttitude(AMBAIBaseCharacter* const InAICharacter) const;
};
