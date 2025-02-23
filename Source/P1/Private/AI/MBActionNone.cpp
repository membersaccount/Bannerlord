#include "AI/MBActionNone.h"
#include "Characters/MBAIBaseCharacter.h"
#include "AI/MBStateManager.h"
#include "Datas/MBEnums.h"
#include "MBDebug.h"

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
		WideDecideAction(InAICharacter);
		break;
	case Distance::TooClose:
		WideDecideAction(InAICharacter);
		break;
	}
}

void MBActionNone::ExecuteAction(AMBAIBaseCharacter* const InAICharacter) const
{
}
