#pragma once

namespace Enums
{
	namespace AI
	{
		namespace States
		{
			enum class Order
			{
				HoldPosition,
				MakeFormation,
				EngageBattle,
			};

			enum class Attitude
			{
				Idle,
				Aggressive,
				Defensive,
			};

			enum class Action
			{
				None,
				Attacking,
				Strike,
				Defending,
				Block,
			};

			enum class Move
			{
				Stop,
				Force,
				Run,
				Charge,
				Chase,
				Lead,
				Avoid,
			};

			enum class Distance
			{
				None,
				Long,
				Short,
				Combat,
				TooClose,
			};

			enum class Direction
			{
				None,
				Up,
				Down,
				Left,
				Right,
				Shield,
			};
		}
	}
}