#pragma once
#include "MBEnums.h"
#include "AI/MBOrder.h"
#include "AI/MBAttitude.h"
#include "AI/MBAction.h"
#include "AI/MBMove.h"

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
			MBOrder* OrderData;
			MBAttitude* AttitudeData;
			MBAction* ActionData;
			MBMove* MoveData;
		};

		struct StateType
		{
			Enums::AI::States::Order OrderType;
			Enums::AI::States::Attitude AttitudeType;
			Enums::AI::States::Action ActionType;
			Enums::AI::States::Move MoveType;
			Enums::AI::States::Direction DirectionType;
		};
	}
}