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
			Enums::AI::States::Attitude AttitudeData;
			Enums::AI::States::Action ActionData;
			Enums::AI::States::Move MoveData;
			Enums::AI::States::Direction DirectionData;
		};
	}
}