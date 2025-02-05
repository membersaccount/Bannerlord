#pragma once
#include "MBEnums.h"

class AMBAIBaseCharacter;

namespace Structs
{
	namespace AI
	{
		struct AIInfoData
		{
			AMBAIBaseCharacter* InfoSelfData;
			FVector InfoLocation;
			FRotator InfoRotation;
			AMBAIBaseCharacter* InfoTargetData;
			int TargetedCount;
		};

		struct State
		{
			Enums::AI::States::Order OrderData;
			Enums::AI::States::ActionStateType AttitudeStateData;
			Enums::AI::States::MoveStateType MoveStateData;
			Enums::AI::States::ActionStateType ActionStateData;
		};
	}
}