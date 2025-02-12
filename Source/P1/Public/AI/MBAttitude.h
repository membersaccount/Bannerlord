#pragma once

class AMBAIBaseCharacter;

class P1_API MBAttitude
{
public:
	virtual ~MBAttitude() = default;

	virtual void DecideAttitude(AMBAIBaseCharacter* InAICharacter) = 0;

public: // Move -> Attitude
	virtual void PassDecideMoveAttitude(AMBAIBaseCharacter* InAICharacter) = 0;

public: 

};
