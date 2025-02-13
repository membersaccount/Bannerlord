#pragma once

class AMBAIBaseCharacter;

class P1_API MBAttitude
{
public:
	virtual ~MBAttitude() = default;

	virtual void DecideAttitude(AMBAIBaseCharacter* const InAICharacter) const = 0;

public: // Move -> Attitude
	virtual void PassDecideMoveAttitude(AMBAIBaseCharacter* const InAICharacter) const = 0;

public: 

};
