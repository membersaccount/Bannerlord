#include "AI/MBActionNone.h"
#include "Characters/MBAIBaseCharacter.h"
#include "AI/MBStateManager.h"
#include "Datas/MBEnums.h"

void MBActionNone::DecideAction(AMBAIBaseCharacter* const InAICharacter) const
{
	switch (InAICharacter->TargetDistance)
	{
	case Distance::None:
		break;
	case Distance::Long:
		break;
	case Distance::Short:
		break;
	case Distance::Combat:
		if (nullptr == InAICharacter->AIInfo->InfoTargetData &&
			true == InAICharacter->AIInfo->InfoTargetData->GetIsDead())
		{

		}
		break;
	case Distance::TooClose:
		break;
	}
}

void MBActionNone::ExecuteAction(AMBAIBaseCharacter* const InAICharacter) const
{
}
