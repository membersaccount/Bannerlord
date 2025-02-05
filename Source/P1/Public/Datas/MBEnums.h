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
				Charge,
			};

			enum class AttitudeStateType
			{
				Idle,
				Aggressive,
				Defensive,
			};

			enum class MoveStateType
			{
				Stop,
				Move,
				Charge,
				Chase,
				Lead,
				Avoid,
			};

			enum class ActionStateType
			{
				None,
				Attacking,
				Strike,
				Defending,
				Block,
			};
		}
	}
}