#include <stdio.h>
#include "RoadPlan.h"
#include <stdlib.h>

int main(_In_ int _Argc, _In_count_(_Argc) _Pre_z_ char ** _Argv, _In_z_ char ** _Env)
{
	CRoadPlan plan;
	tagRoad tRoad;
	long lRet = -1;
	int nDirection = -1;
	TAGPOS pos;
	tagCrossing tCrossing;
	tagRoad tNextRoad;

// 	bool bRet = plan.StartCodeCardArriveEndCard(11, 42);
// 	bRet = plan.StartCodeCardArriveEndCard(55, 88);
// 	lRet = plan.GetRoadInfoFromCardID(64, tRoad);
// 	lRet = plan.GetRoadInfoFromCardID(45, tRoad);
// 	lRet = plan.GetCardPos(tRoad.nEndCard, pos);
// 	lRet = plan.GetRoadEndCrossing(tRoad, tCrossing);
// 	lRet = plan.EndPointIsInThisRoad(88, tRoad.nID);
// 	lRet = plan.GetRunDirection(tRoad, pos, nDirection);
// 	lRet = plan.GetNextRoad(tCrossing, nDirection, tNextRoad);

	//lRet = plan.RoadPlan(87, 94);
	//lRet = plan.RoadPlan(87, 123);
	lRet = plan.RoadPlan(76, 124);
	plan.GetPlan();
	plan.PrintPlan();
	
	system("pause");
	return 0;
};