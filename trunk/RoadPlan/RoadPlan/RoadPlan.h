#pragma once

#include "SandTableDB.h"
#include <string>
#include <vector>
#include <list>

#define EAST  0  //东
#define SOUTH 1  //南
#define WEST  2  //西
#define NORTH 3  //北

using namespace std;

typedef enum TagPlanType
{
	PLAN_TYPE_ROAD = 1,
	PLAN_TYPE_CROSSING =2,
	PLAN_TYPE_OTHER = 99
}TAGPLANTYPE;

typedef struct TagPlanRoad
{
	string strDescription; //直行描述
	int nRoadID;
	TagPlanRoad()
	{
		nRoadID = -1;
		strDescription = "";
	}
	~TagPlanRoad()
	{
		nRoadID = -1;
		strDescription = "";
	}
}TAGPLANROAD;

typedef struct TagPlanCrossing 
{
	string strDescription;  //转向描述
	int nDirectLine;		//转向的线标号
	int nCrossingID;		//十字口编号
	void Reset()
	{
		strDescription = "";
		nDirectLine = -1;
		nCrossingID = -1;
	}

	TagPlanCrossing()
	{
		Reset();
	}

	~TagPlanCrossing()
	{
		Reset();
	}
}TAGPLANCROSSING;

typedef struct TagPlan
{
	TAGPLANTYPE type;				//本节点类型 道路 或者 路口
	TAGPLANROAD tPlanRoad;
	TAGPLANCROSSING tPlanCrossing;
	void Reset()
	{
		type = PLAN_TYPE_OTHER;
	}
	TagPlan()
	{
		Reset();
	}
	~TagPlan()
	{
		Reset();
	}
}TAGPLAN;

typedef struct TagPos
{
	int x;
	int y;
	void Reset()
	{
		x = -1;
		y = -1;
	}

	TagPos()
	{
		Reset();
	}

	~TagPos()
	{
		Reset();
	}
}TAGPOS;



class CRoadPlan
{
public:
	CRoadPlan(void);
	~CRoadPlan(void);

public:
	long RoadPlan(const int nStartCard, const int nEndCard);
	bool StartCodeCardArriveEndCard(const int nStartCard, const int nEndCard);
	long GetRoadInfoFromCardID(const int nCardID, tagRoad & tRoad);
	long GetCardPos(const int nCardID, TAGPOS & tPos);
	long GetRunDirection(const tagRoad & tRoad, const TAGPOS tEndPos, int & nDirection);
	long GetRoadEndCrossing(const tagRoad & tRoad, tagCrossing & tCrossing);
	long GetNextRoad(const tagCrossing & tCrossing, const int & nDirection, tagRoad & tNextRoad);
	bool EndPointIsInThisRoad(const int nCardID, const int nRoadID);
	const char * GetErrStr();
	long AddTagPlan(const tagCrossing * pCrossing, const tagRoad * pRoad, const int nDirection, const bool bStart = false, const bool bEnd = false);
	void PrintPlan();
	long GetCardFromPos(TAGPOS pos, list<tagCard> & lstCard);
	const list<TagPlan> & GetPlan();
	bool IsRoadInList(const tagRoad & tRoad);

private:
	CSandTableDB m_db;
	string m_ErrStr;
	int m_nStartCardID;
	int m_nEndCardID;
	list<TagPlan> m_lstPlan;
};

