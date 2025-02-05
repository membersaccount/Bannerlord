#pragma once

class AMBAIBaseCharacter;

namespace Structs
{
	namespace AI
	{
		namespace InfoData
		{
			struct AIInfoData
			{
				AMBAIBaseCharacter* AIInfo;
				FVector InfoLocation;
				FRotator InfoRotation;
				AMBAIBaseCharacter* TargetInfo;
				int TargetedCount;
			};
		}
	}
}