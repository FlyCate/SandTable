#include "RoadPlan.h"
#include <list>

using namespace std;

#define DB_NAME "SandTable.db"

#define STRAIGHT 0
#define RIGHT	 1

#define WAI_HUAN_ID 1 //外环路 ID

#define ROAD_TO_CROSSING_RIGHT_POS 80 //道路终点和右转的坐标差 阈值

#define PRINTF()\
printf("[%s][%s][%d]", __FILE__, __FUNCTION__, __LINE__);\

#define OPENDB()\
bRet = m_db.OpenDB(DB_NAME);\
if (bRet == false)\
{\
	PRINTF();\
	printf("打开%s文件失败\n", DB_NAME);\
	return -1;\
}

#define OPENDB_BOOL()\
	bRet = m_db.OpenDB(DB_NAME);\
	if (bRet == false)\
{\
	PRINTF();\
	printf("打开%s文件失败\n", DB_NAME);\
	return false;\
}

#define CLOSEDB()\
m_db.CloseDB();

CRoadPlan::CRoadPlan(void)
{
}


CRoadPlan::~CRoadPlan(void)
{
}

long CRoadPlan::RoadPlan(const int nStartCard, const int nEndCard)
{
	bool bRet = false;
	long lRet = -1;

	//判断是否 起始点可以道道终点
	bRet = StartCodeCardArriveEndCard(nStartCard, nEndCard);
	if (bRet == false)
	{
		return -1;
	}

	m_nStartCardID = nStartCard;
	m_nEndCardID = nEndCard;
	m_lstPlan.clear();

	//查找起始点坐标
	TAGPOS tStartPos;
	lRet = GetCardPos(nStartCard, tStartPos);
	if (lRet != 0)
	{
		m_ErrStr = "查找起始点坐标错误";
		return -1;
	}

	//查找终点坐标
	TAGPOS tEndPos;
	lRet = GetCardPos(nEndCard, tEndPos);
	if (lRet != 0)
	{
		m_ErrStr = "查找终点坐标错误";
		return -1;
	}

	//查找起始点所在道路
	tagRoad tStartRoad;
	lRet = GetRoadInfoFromCardID(nStartCard, tStartRoad);
	if (lRet != 0)
	{
		m_ErrStr = "查找起始点所在道路错误";
		return -1;
	}

	//查找终点所在道路
	tagRoad tEndRoad;
	lRet = GetRoadInfoFromCardID(nEndCard, tEndRoad);
	if (lRet != 0)
	{
		m_ErrStr = "查找终点所在道路错误";
		return -1;
	}

	//判断起始点和终点是否在一条路上
	if (tStartRoad.nID == tEndRoad.nID)
	{
		//在一条路上 结束本次规划
		AddTagPlan(NULL, &tStartRoad, STRAIGHT, true, true);
		return 0;
	}

	//继续行驶到道路终点
	AddTagPlan(NULL, &tStartRoad, STRAIGHT, true);

	tagRoad tCurrentRoad = tStartRoad;
	tagCrossing tCurrentCrossing;
	while (1)
	{
		//查找道路终点对应路口信息
		lRet = GetRoadEndCrossing(tCurrentRoad, tCurrentCrossing);
		if (lRet != 0)
		{
			m_ErrStr = "查找道路终点对应路口信息错误";
			return -1;
		}

		tagRoad tRightRoad;
		//当前路口为4号路口 北方向
		if (tCurrentCrossing.nCrossingID == 4 && tCurrentCrossing.nDirection == NORTH)
		{
			//如果终点在停车位1 2 3时，当前路口为4号路口
			if (tEndRoad.strRoadName.compare("停车位1") == 0)
			{
				//在一条路上 结束本次规划
				TAGPLAN tPlan;
				tPlan.type = PLAN_TYPE_CROSSING;
				tPlan.tPlanCrossing.strDescription = "在 ";
				tPlan.tPlanCrossing.strDescription += tCurrentCrossing.strCrossingName.c_str();
				tPlan.tPlanCrossing.strDescription += " 右转";
				tPlan.tPlanCrossing.nDirectLine = 5;
				tPlan.tPlanCrossing.nCrossingID = tCurrentCrossing.nCrossingID;
				m_lstPlan.push_back(tPlan);
				AddTagPlan(NULL, &tEndRoad, STRAIGHT, false, true);
				return 0;
			}
			else if (tEndRoad.strRoadName.compare("停车位2") == 0)
			{
				//在一条路上 结束本次规划
				TAGPLAN tPlan;
				tPlan.type = PLAN_TYPE_CROSSING;
				tPlan.tPlanCrossing.strDescription = "在 ";
				tPlan.tPlanCrossing.strDescription += tCurrentCrossing.strCrossingName.c_str();
				tPlan.tPlanCrossing.strDescription += " 右转";
				tPlan.tPlanCrossing.nDirectLine = 4;
				tPlan.tPlanCrossing.nCrossingID = tCurrentCrossing.nCrossingID;
				m_lstPlan.push_back(tPlan);
				AddTagPlan(NULL, &tEndRoad, STRAIGHT, false, true);
				return 0;
			}
			else if (tEndRoad.strRoadName.compare("停车位3") == 0)
			{
				//在一条路上 结束本次规划
				TAGPLAN tPlan;
				tPlan.type = PLAN_TYPE_CROSSING;
				tPlan.tPlanCrossing.strDescription = "在 ";
				tPlan.tPlanCrossing.strDescription += tCurrentCrossing.strCrossingName.c_str();
				tPlan.tPlanCrossing.strDescription += " 右转";
				tPlan.tPlanCrossing.nDirectLine = 3;
				tPlan.tPlanCrossing.nCrossingID = tCurrentCrossing.nCrossingID;
				m_lstPlan.push_back(tPlan);
				AddTagPlan(NULL, &tEndRoad, STRAIGHT, false, true);
				return 0;
			}
			else
			{
				//终点 不在停车位直接查找直行道路
				lRet = -1;
			}
		}
		else //不为4号路口 北方向
		{
			//查找右转道路
			lRet = GetNextRoad(tCurrentCrossing, RIGHT, tRightRoad);
		}

		if (lRet != 0)
		{
			m_ErrStr = "右转道路不存在";
			printf("右转道路不存在\n");

			//右转道路不存在直接直行
			lRet = GetNextRoad(tCurrentCrossing, STRAIGHT, tCurrentRoad);
			if (lRet != 0)
			{
				m_ErrStr = "直行道路不存在";
				return -1;
			}
		}
		else
		{
			//查找到右转道路
			//判断右转对应的道路是不是终点所在道路
			if (tRightRoad.nID == tEndRoad.nID)
			{
				//在一条路上 结束本次规划
				AddTagPlan(&tCurrentCrossing, NULL, RIGHT);
				AddTagPlan(NULL, &tRightRoad, STRAIGHT, false, true);
				return 0;
			}

			//判断坐标关系确定行驶方向
			int nDirection = -1;
			lRet = GetRunDirection(tCurrentRoad, tEndPos, nDirection);
			if (lRet != 0)
			{
				m_ErrStr = "判断坐标关系确定行驶方向错误";
				return -1;
			}

			//判断右转道路 不在已走道路中
			//如果方向为右转 则不用重新读取
			if ((nDirection == RIGHT)&&(IsRoadInList(tRightRoad) == false))
			{
				//赋值后直接跳过判断
				AddTagPlan(&tCurrentCrossing, NULL, RIGHT);
				AddTagPlan(NULL, &tRightRoad, STRAIGHT);
				tCurrentRoad = tRightRoad;
				continue;
			}

			//如果方向为直行 则重新读取
			//获取直行的道路
			lRet = GetNextRoad(tCurrentCrossing, STRAIGHT, tCurrentRoad);
			if (lRet != 0)
			{
				//直行道路不存在 只能右转 赋值后跳过判断
				m_ErrStr = "直行道路不存在";
				AddTagPlan(&tCurrentCrossing, NULL, RIGHT);
				AddTagPlan(NULL, &tRightRoad, STRAIGHT);
				tCurrentRoad = tRightRoad;
				continue;
			}

			//判断直行道路 不在已走道路中
			if (IsRoadInList(tCurrentRoad) == true)
			{
				//右转
				AddTagPlan(&tCurrentCrossing, NULL, RIGHT);
				AddTagPlan(NULL, &tRightRoad, STRAIGHT);
				tCurrentRoad = tRightRoad;
				continue;
			}

			//继续进行判断
		}

		AddTagPlan(&tCurrentCrossing, NULL, STRAIGHT);

		//判断直行对应的道路是不是终点所在道路
		if (tCurrentRoad.nID == tEndRoad.nID)
		{
			//在一条路上 结束本次规划
			AddTagPlan(NULL, &tCurrentRoad, STRAIGHT, false, true);
			return 0;
		}
		AddTagPlan(NULL, &tCurrentRoad, STRAIGHT);
	}
}

bool CRoadPlan::StartCodeCardArriveEndCard(const int nStartCard, const int nEndCard)
{
	bool bRet = false;

	OPENDB_BOOL();

	tagRoad tStartRoad;
	tagRoad tEndRoad;
	bRet = m_db.FindRoadFromCard(nStartCard, tStartRoad);
	if (bRet == false)
	{
		PRINTF();
		printf("查找起始点道路信息错误\n");
		m_ErrStr = "查找起始点道路信息错误";
		CLOSEDB();
		return false;
	}

	bRet = m_db.FindRoadFromCard(nEndCard, tEndRoad);
	if (bRet == false)
	{
		PRINTF();
		printf("查找结束点道路信息错误\n");
		m_ErrStr = "查找结束点道路信息错误";
		CLOSEDB();
		return false;
	}

	CLOSEDB();

	if (tStartRoad.nID == WAI_HUAN_ID)
	{
		if (tEndRoad.nID == WAI_HUAN_ID)
		{
			printf("TRUE, 都在外环路\n");
			return true;
		}
		else
		{
			printf("FALSE 起始点在外环路，结束点在%s\n", tEndRoad.strRoadName.c_str());
			m_ErrStr = "FALSE 起始点在外环路，结束点在";
			m_ErrStr += tEndRoad.strRoadName.c_str();
			return false;
		}
	}
	else
	{
		if (tEndRoad.nID != WAI_HUAN_ID)
		{
			printf("TRUE 都不在外环路, 起始点在%s, 结束点在%s\n", tStartRoad.strRoadName.c_str(), tEndRoad.strRoadName.c_str());
			return true;
		}
		else
		{
			printf("FALSE 起始点在%s，结束点在外环路\n", tStartRoad.strRoadName.c_str());
			m_ErrStr = "FALSE 起始点在";
			m_ErrStr += tStartRoad.strRoadName.c_str();
			m_ErrStr += "，结束点在外环路";
			return false;
		}
	}

	return bRet;
}

long CRoadPlan::GetRoadInfoFromCardID(const int nCardID, tagRoad & tRoad)
{
	bool bRet = false;
	long lRet = 0;

	OPENDB();

	bRet = m_db.FindRoadFromCard(nCardID, tRoad);
	if (bRet == false)
	{
		PRINTF();
		printf("查找道路信息错误\n");
		m_ErrStr = "查找道路信息错误";
		lRet = -1;
	}

	CLOSEDB();

	return lRet;
}

long CRoadPlan::GetCardPos(const int nCardID, TAGPOS & tPos)
{
	bool bRet = false;
	long lRet = 0;

	OPENDB();

	tagCard tCard;
	bRet = m_db.FindCardInfo(nCardID, tCard);
	if (bRet == false)
	{
		PRINTF();
		printf("查找卡片信息错误\n");
		m_ErrStr = "查找卡片信息错误";
		lRet = -1;
	}
	else
	{
		tPos.x = tCard.nXpos;
		tPos.y = tCard.nYpos;
	}

	CLOSEDB();

	return lRet;
}

long CRoadPlan::GetRunDirection(const tagRoad & tRoad, const TAGPOS tEndPos, int & nDirection)
{
	long lRet = -1;
	TAGPOS tRoadEndPos;

	lRet = GetCardPos(tRoad.nEndCard, tRoadEndPos);
	if (lRet != 0)
	{
		printf("查找道路终点坐标失败！\n");
		m_ErrStr = "查找道路终点坐标失败";
		return lRet;
	}
	
	lRet = 0;
	switch (tRoad.nEndCrossingDirect)
	{
	case EAST://由东向西
		{
			if (tRoadEndPos.x - ROAD_TO_CROSSING_RIGHT_POS < tEndPos.x)
			{
				nDirection = RIGHT;
			}
			else
			{
				nDirection = STRAIGHT;
			}
		}
		break;
	case SOUTH://由南向北
		{
			if (tRoadEndPos.y + ROAD_TO_CROSSING_RIGHT_POS > tEndPos.y)
			{
				nDirection = RIGHT;
			}
			else
			{
				nDirection = STRAIGHT;
			}
		}
		break;
	case WEST://由西向东
		{
			if (tRoadEndPos.x + ROAD_TO_CROSSING_RIGHT_POS > tEndPos.x)
			{
				nDirection = RIGHT;
			}
			else
			{
				nDirection = STRAIGHT;
			}
		}
		break;
	case NORTH://由北向南
		{
			if (tRoadEndPos.y - ROAD_TO_CROSSING_RIGHT_POS < tEndPos.y)
			{
				nDirection = RIGHT;
			}
			else
			{
				nDirection = STRAIGHT;
			}
		}
		break;
	default:
		m_ErrStr = "方向错误";
		lRet = -1;
		break;
	}

	return lRet;
}

long CRoadPlan::GetRoadEndCrossing(const tagRoad & tRoad, tagCrossing & tCrossing)
{
	bool bRet = false;
	long lRet = 0;

	OPENDB();

	bRet = m_db.FindCrossingInfo(tRoad, tCrossing);
	if (bRet == false)
	{
		PRINTF();
		printf("查找路口信息错误\n");
		m_ErrStr = "查找路口信息错误";
		lRet = -1;
	}

	CLOSEDB();

	return lRet;
}

long CRoadPlan::GetNextRoad(const tagCrossing & tCrossing, const int & nDirection, tagRoad & tNextRoad)
{
	bool bRet = false;
	long lRet = 0;

	OPENDB();

	switch (nDirection)
	{
	case STRAIGHT:
		{
			bRet = m_db.FindRoadFromCrossing(tCrossing.nCrossingID, tCrossing.nStraightDirect, tNextRoad);
			if (bRet == false)
			{
				PRINTF();
				printf("查找路口信息错误\n");
				m_ErrStr = "查找路口信息错误";
				lRet = -1;
			}
		}
		break;
	case RIGHT:
		{
			bRet = m_db.FindRoadFromCrossing(tCrossing.nCrossingID, tCrossing.nDextroversionDirect, tNextRoad);
			if (bRet == false)
			{
				PRINTF();
				printf("查找路口信息错误\n");
				m_ErrStr = "查找路口信息错误";
				lRet = -1;
			}
		}
		break;
	default:
		{
			printf("方向类型错误\n");
			m_ErrStr = "方向类型错误";
			return -1;
		}
		break;
	}
	
	CLOSEDB();

	return lRet;
}

bool CRoadPlan::EndPointIsInThisRoad(const int nCardID, const int nRoadID)
{
	bool bRet = false;

	OPENDB_BOOL();
	list<tagRoadCard> lstCard;
	bRet = m_db.ReadDB(nRoadID, lstCard);
	if (bRet == false)
	{
		PRINTF();
		printf("查找道路信息错误\n");
		m_ErrStr = "查找道路信息错误";
	}
	else
	{
		bRet = false;
		m_ErrStr = "查找不到对应的nCardID";
		for (auto & it = lstCard.begin(); it != lstCard.end(); it++)
		{
			if (it->nCardID == nCardID)
			{
				bRet = true;
				printf("查找到对应的nCardID\n");
			}
		}
	}

	CLOSEDB();

	return bRet;
}

const char * CRoadPlan::GetErrStr()
{
	return m_ErrStr.c_str();
}

long CRoadPlan::AddTagPlan(const tagCrossing * pCrossing, const tagRoad * pRoad, const int nDirection, const bool bStart, const bool bEnd)
{
	TagPlan tPlan;

	if (pCrossing != NULL)
	{
		tPlan.type = PLAN_TYPE_CROSSING;
		tPlan.tPlanCrossing.strDescription = "在 ";
		tPlan.tPlanCrossing.strDescription += pCrossing->strCrossingName.c_str();
		tPlan.tPlanCrossing.strDescription += nDirection == STRAIGHT? " 直行" : " 右转";
		tPlan.tPlanCrossing.nDirectLine = nDirection == STRAIGHT? pCrossing->nStraightLine : pCrossing->nDextroversionLine;
		tPlan.tPlanCrossing.nCrossingID = pCrossing->nCrossingID;
		printf("%s\n", tPlan.tPlanCrossing.strDescription.c_str());
	}

	if (pRoad != NULL)
	{
		tPlan.type = PLAN_TYPE_ROAD;
		if (bStart == true)
		{
			tPlan.tPlanRoad.strDescription = "从起始点出发 沿 ";
		}
		else
		{
			tPlan.tPlanRoad.strDescription = "沿 ";
		}
		
		tPlan.tPlanRoad.strDescription += pRoad->strRoadName.c_str();
		tPlan.tPlanRoad.strDescription += " 行驶";
		if (bEnd == true)
		{
			tPlan.tPlanRoad.strDescription += " 行驶到达终点";
		}
		tPlan.tPlanRoad.nRoadID = pRoad->nID;

		printf("%s\n", tPlan.tPlanRoad.strDescription.c_str());
	}

	m_lstPlan.push_back(tPlan);

	return 0;
}

void CRoadPlan::PrintPlan()
{
	for (auto it = m_lstPlan.begin(); it != m_lstPlan.end(); it++)
	{
		if (it->type == PLAN_TYPE_CROSSING)
		{
			printf("%s\n", it->tPlanCrossing.strDescription.c_str());
		}

		if (it->type == PLAN_TYPE_ROAD)
		{
			printf("%s\n", it->tPlanRoad.strDescription.c_str());
		}
	}
}

long CRoadPlan::GetCardFromPos(TAGPOS pos, list<tagCard> & lstCard)
{

	bool bRet = false;

	OPENDB_BOOL();
	
	bRet = m_db.FindCardFromPos(pos.x, pos.y, lstCard);
	if (bRet == false)
	{
		PRINTF();
		printf("查找Card信息错误\n");
		m_ErrStr = "查找Card信息错误";
	}

	CLOSEDB();

	return bRet;
}

const list<TagPlan> & CRoadPlan::GetPlan()
{
	TagPlan tStartRoad = m_lstPlan.front();
	bool bRepeat = false;

	auto it = m_lstPlan.begin();
	it++;
	for (; it != m_lstPlan.end(); it++)
	{
		if (it->type == PLAN_TYPE_ROAD)
		{
			if (it->tPlanRoad.nRoadID == tStartRoad.tPlanRoad.nRoadID)
			{
				bRepeat = true;
			}
		}
	}

	if (bRepeat == true)
	{
		TagPlan tCurrentPlan;
		do 
		{
			m_lstPlan.pop_front();
			tCurrentPlan = m_lstPlan.front();
		} while ((tCurrentPlan.type != PLAN_TYPE_ROAD) || (tCurrentPlan.tPlanRoad.nRoadID != tStartRoad.tPlanRoad.nRoadID));
	}

	m_lstPlan.begin()->tPlanRoad.strDescription = tStartRoad.tPlanRoad.strDescription;

	return m_lstPlan;
}

bool CRoadPlan::IsRoadInList(const tagRoad & tRoad)
{
	for (auto it = m_lstPlan.begin(); it != m_lstPlan.end(); it++)
	{
		if (it->type == PLAN_TYPE_ROAD)
		{
			if (tRoad.nID == it->tPlanRoad.nRoadID)
			{
				return true;
			}
		}
	}

	return false;
}
