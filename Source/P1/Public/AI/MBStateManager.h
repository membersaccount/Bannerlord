#pragma once
#include "AI/MBOrderHoldPosition.h"
#include "AI/MBOrderMakeFormation.h"
#include "AI/MBOrderEngageBattle.h"

#include "AI/MBAttitudeIdle.h"
#include "AI/MBAttitudeAggressive.h"
#include "AI/MBAttitudeDefensive.h"

#include "AI/MBActionNone.h"
#include "AI/MBActionAttacking.h"
#include "AI/MBActionStrike.h"
#include "AI/MBActionDefending.h"
#include "AI/MBActionBlock.h"

#include "AI/MBMoveStop.h"
#include "AI/MBMoveForce.h"
#include "AI/MBMoveRun.h"
#include "AI/MBMoveCharge.h"
#include "AI/MBMoveChase.h"
#include "AI/MBMoveLead.h"
#include "AI/MBMoveWalk.h"
#include "AI/MBMoveAvoid.h"

class P1_API MBStateManager
{
public:
	MBOrderHoldPosition ManagerOrderHoldPosition;
	MBOrderMakeFormation ManagerOrderMakeFormation;
	MBOrderEngageBattle ManagerOrderEngageBattle;

	MBAttitudeIdle ManagerAttitudeIdle;
	MBAttitudeAggressive ManagerAttitudeAggressive;
	MBAttitudeDefensive ManagerAttitudeDefensive;
	
	MBActionNone ManagerActionNone;
	MBActionAttacking ManagerActionAttacking;
	MBActionStrike ManagerActionStrike;
	MBActionDefending ManagerActionDefending;
	MBActionBlock ManagerActionBlock;

	MBMoveStop ManagerMoveStop;
	MBMoveForce ManagerMoveForce;
	MBMoveRun ManagerMoveRun;
	MBMoveCharge ManagerMoveCharge;
	MBMoveChase ManagerMoveChase;
	MBMoveLead ManagerMoveLead;
	MBMoveWalk ManagerMoveWalk;
	MBMoveAvoid ManagerMoveAvoid;
};
	