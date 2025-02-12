#pragma once
#include "AI/MBAttitude.h"

class P1_API MBAttitudeIdle : public MBAttitude
{
public:
	virtual ~MBAttitudeIdle() {}

	virtual void DecideAttitude(AMBAIBaseCharacter* InAICharacter) override;

public: 
	virtual void PassDecideMoveAttitude(AMBAIBaseCharacter* InAICharacter);
};
