#pragma once
#include "CppSQLite3.h"
#include <string>
#include <list>

using namespace std;

int GBK2UTF8(char *szGbk,char *szUtf8,int Len);
int UTF82GBK(char *szUtf8,char *szGbk,int Len);

enum TABLE_NAME
{
	TABLE_CARD = 1,
	TABLE_ROAD = 2,
	TABLE_CROSSING = 3,
	TABLE_STATION = 4,
	TABLE_ROADCARD = 5,
	TABLE_OTHER = 99
};

struct tagCard
{
	int nID;
	int nCardNum;
	int nXpos;
	int nYpos;
	
	void CleanData()
	{
		nID = 0;
		nCardNum = 0;
		nXpos = 0;
		nYpos = 0;
	}

	tagCard()
	{
		CleanData();
	}

	~tagCard()
	{
		CleanData();
	}
};

struct tagCrossing
{
	int nID;
	string strCrossingName;
	int nCrossingID;
	int nDirection;
	int nStraightLine;
	int nStraightDirect;
	int nDextroversionLine;
	int nDextroversionDirect;

	void CleanData()
	{
		nID = 0;
		strCrossingName = "";
		nCrossingID = 0;
		nDirection = 0;
		nStraightLine = 0;
		nStraightDirect = 0;
		nDextroversionLine = 0;
		nDextroversionDirect = 0;
	}

	tagCrossing()
	{
		CleanData();
	}

	~tagCrossing()
	{
		CleanData();
	}
};

struct tagStation
{
	int nID;
	string strStationName;
	int nRoadID;
	int nRoadDirect;
	int nStationCard;
	int nStationGroup;

	void CleanData()
	{
		nID = 0;
		strStationName = "";
		nRoadID = 0;
		nRoadDirect = 0;
		nStationCard = 0;
		nStationGroup = 0;
	}

	tagStation()
	{
		CleanData();
	}

	~tagStation()
	{
		CleanData();
	}
};

struct tagRoad
{
	int nID;
	string strRoadName;
	int nDirection;
	int nStartCard;
	int nStartCrossingCode;
	int nStartCrossingDirect;
	int nEndCard;
	int nEndCrossingCode;
	int nEndCrossingDirect;

	void CleanData()
	{
		nID = 0;
		strRoadName = "";
		nDirection = 0;
		nStartCard = 0;
		nStartCrossingCode = 0;
		nStartCrossingDirect = 0;
		nEndCard = 0;
		nEndCrossingCode = 0;
		nEndCrossingDirect = 0;
	}

	tagRoad()
	{
		CleanData();
	}

	~tagRoad()
	{
		CleanData();
	}
};

struct tagRoadCard 
{
	int nID;
	int nRoadID;
	int nCardID;

	void Reset()
	{
		nID = -1;
		nRoadID = -1;
		nCardID = -1;
	}

	tagRoadCard()
	{
		Reset();
	}

	~tagRoadCard()
	{
		Reset();
	}
};

class CSandTableDB
{
public:
	CSandTableDB();
	~CSandTableDB();

public:
	bool OpenDB(const char * pFilePath);//打开DB文件
	bool CloseDB();//关闭DB文件
	bool WriteDB(tagRoad tRoad);
	bool WriteDB(tagStation tStation);
	bool WriteDB(tagCrossing tCrossing);
	bool WriteDB(tagCard tCard);
	bool WriteDB(tagRoadCard tRoadCard);
	bool ReadDB(list<tagRoad> & listRoad);
	bool ReadDB(list<tagStation> & listStation);
	bool ReadDB(list<tagCrossing> & listCrossing);
	bool ReadDB(list<tagCard> & listCard);
	bool ReadDB(int nRoadID, list<tagRoadCard>& listRoadCard);
	bool DeleteDB(tagRoad tRoad);
	bool DeleteDB(tagStation tStation);
	bool DeleteDB(tagCrossing tCrossing);
	bool DeleteDB(tagCard tCard);
	bool DeleteDB(tagRoadCard tRoadCard);

private:
	bool ExcuteSQL(const char * pSQL);						//SQL语句执行
	bool IsTableExist(const TABLE_NAME enumTableName);		//查找表是否存在
	bool CreateTable(const TABLE_NAME enumTableName);		//创建表

private:
	CppSQLite3DB * m_DB;
};