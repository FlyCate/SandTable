#include "StdAfx.h"
#include "SandTableDB.h"
#include <iostream>
#include <sstream>

//�ж����ݿ���Ƿ����  �������򴴽�  ����ʧ�����˳�
#define CREATE_TABLE(x) \
if (false == CreateTable(x)) \
{ \
	cerr << "CreateTable "#x" error!" << endl; \
	return false; \
}

#define UTF8_TO_GBK(x)\
char szTemp[64] = {0};\
UTF82GBK((char *)x.c_str(), szTemp, 64);\
x = szTemp;

#define GBK_TO_UTF8(x)\
char szTemp[64] = {0};\
GBK2UTF8((char *)x.c_str(), szTemp, 64);\
x = szTemp;

CSandTableDB::CSandTableDB()
{
	m_DB = NULL;

	m_DB = new CppSQLite3DB();
	if (NULL == m_DB)
	{
		cout << "new CppSQLite3DB error!" << endl;
		return;
	}
}


CSandTableDB::~CSandTableDB()
{
	if (NULL != m_DB)
	{
		delete m_DB;
		m_DB = NULL;
	}
}

bool CSandTableDB::OpenDB(const char * pFilePath)
{
	if (NULL == m_DB)
	{
		return false;
	}

	try
	{
		m_DB->open(pFilePath);
	}
	catch (CppSQLite3Exception& e)
	{
		cerr << e.errorCode() << ":" << e.errorMessage() << endl;
		return false;
	}

	return true;
}

bool CSandTableDB::CloseDB()
{
	if (NULL == m_DB)
	{
		return false;
	}

	try
	{
		m_DB->close();
	}
	catch (CppSQLite3Exception& e)
	{
		cerr << e.errorCode() << ":" << e.errorMessage() << endl;
		return false;
	}

	return true;
}

bool CSandTableDB::WriteDB(tagRoad tRoad)
{
	CREATE_TABLE(TABLE_ROAD);
	stringstream strSQL;

	strSQL << "INSERT INTO T_RoadInfo (RoadName, Direction, StartCard, StartCrossingCode, StartCrossingDirect, EndCard, EndCrossingCode, EndCrossingDirect) VALUES (\"";
	GBK_TO_UTF8(tRoad.strRoadName);
	strSQL << tRoad.strRoadName;
	strSQL << "\",";
	strSQL << tRoad.nDirection;
	strSQL << ",";
	strSQL << tRoad.nStartCard;
	strSQL << ",";
	strSQL << tRoad.nStartCrossingCode;
	strSQL << ",";
	strSQL << tRoad.nStartCrossingDirect;
	strSQL << ",";
	strSQL << tRoad.nEndCard;
	strSQL << ",";
	strSQL << tRoad.nEndCrossingCode;
	strSQL << ",";
	strSQL << tRoad.nEndCrossingDirect;
	strSQL << ")";

	//����ִ�н��
	return ExcuteSQL(strSQL.str().c_str());
}

bool CSandTableDB::WriteDB(tagStation tStation)
{
	CREATE_TABLE(TABLE_STATION);
	stringstream strSQL;

	strSQL << "INSERT INTO T_Station (StationName, RoadID, RoadDirect, StationCard, StationGroup) VALUES (\"";
	GBK_TO_UTF8(tStation.strStationName);
	strSQL << tStation.strStationName;
	strSQL << "\",";
	strSQL << tStation.nRoadID;
	strSQL << ",";
	strSQL << tStation.nRoadDirect;
	strSQL << ",";
	strSQL << tStation.nStationCard;
	strSQL << ",";
	strSQL << tStation.nStationGroup;
	strSQL << ")";

	//����ִ�н��
	return ExcuteSQL(strSQL.str().c_str());
}

bool CSandTableDB::WriteDB(tagCrossing tCrossing)
{
	CREATE_TABLE(TABLE_CROSSING);
	stringstream strSQL;

	strSQL << "INSERT INTO T_Crossing (CrossingName, CrossingID, Direction, StraightLine, StraightDirect, DextroversionLine, DextroversionDirect) VALUES (\"";
	GBK_TO_UTF8(tCrossing.strCrossingName);
	strSQL << tCrossing.strCrossingName;
	strSQL << "\",";
	strSQL << tCrossing.nCrossingID;
	strSQL << ",";
	strSQL << tCrossing.nDirection;
	strSQL << ",";
	strSQL << tCrossing.nStraightLine;
	strSQL << ",";
	strSQL << tCrossing.nStraightDirect;
	strSQL << ",";
	strSQL << tCrossing.nDextroversionLine;
	strSQL << ",";
	strSQL << tCrossing.nDextroversionDirect;
	strSQL << ")";

	//����ִ�н��
	return ExcuteSQL(strSQL.str().c_str());
}

bool CSandTableDB::WriteDB(tagCard tCard)
{
	CREATE_TABLE(TABLE_CARD);
	stringstream strSQL;

	strSQL << "INSERT INTO T_Card (Card, Xpos, Ypos) VALUES (";
	strSQL << tCard.nCardNum;
	strSQL << ",";
	strSQL << tCard.nXpos;
	strSQL << ",";
	strSQL << tCard.nYpos;
	strSQL << ")";

	//����ִ�н��
	return ExcuteSQL(strSQL.str().c_str());
}

bool CSandTableDB::WriteDB(tagRoadCard tRoadCard)
{
	CREATE_TABLE(TABLE_ROADCARD);
	stringstream strSQL;

	strSQL << "INSERT INTO T_RoadCard (RoadID, CardID) VALUES (";
	strSQL << tRoadCard.nRoadID;
	strSQL << ",";
	strSQL << tRoadCard.nCardID;
	strSQL << ")";

	//����ִ�н��
	return ExcuteSQL(strSQL.str().c_str());
}

#define SELECT_DATA_BEGIN(x, y)\
if (false == IsTableExist(y))\
{\
	return false;\
}\
strSql << "SELECT * FROM "#x;\
try\
{\
	CppSQLite3Table resTable = m_DB->getTable(strSql.str().c_str());\
	for (int row = 0; row < resTable.numRows(); row++)\
	{\
		resTable.setRow(row);

#define SELECT_DATA_END()\
	}\
}\
catch (CppSQLite3Exception& e)\
{\
	cout << "SQL: " << strSql.str().c_str() << endl;\
	cerr << e.errorCode() << ":" << e.errorMessage() << endl;\
	return false;\
}

bool CSandTableDB::ReadDB(list<tagRoad>& listRoad)
{
	stringstream strSql;
	SELECT_DATA_BEGIN("T_RoadInfo", TABLE_ROAD);

	//��������ȡ����
	tagRoad RoadInfo;
	RoadInfo.nID = atoi(resTable.fieldValue(0));
	RoadInfo.strRoadName = resTable.fieldValue(1);
	UTF8_TO_GBK(RoadInfo.strRoadName);
	RoadInfo.nDirection = atoi(resTable.fieldValue(2));
	RoadInfo.nStartCard = atoi(resTable.fieldValue(3));
	RoadInfo.nStartCrossingCode = atoi(resTable.fieldValue(4));
	RoadInfo.nStartCrossingDirect = atoi(resTable.fieldValue(5));
	RoadInfo.nEndCard = atoi(resTable.fieldValue(6));
	RoadInfo.nEndCrossingCode = atoi(resTable.fieldValue(7));
	RoadInfo.nEndCrossingDirect = atoi(resTable.fieldValue(8));

	//���뵽list��
	listRoad.push_back(RoadInfo);

	SELECT_DATA_END();

	return true;
}

bool CSandTableDB::ReadDB(list<tagStation> & listStation)
{
	stringstream strSql;
	SELECT_DATA_BEGIN("T_Station", TABLE_STATION);

	//��������ȡ����
	tagStation Station;
	Station.nID = atoi(resTable.fieldValue(0));
	Station.strStationName = resTable.fieldValue(1);
	UTF8_TO_GBK(Station.strStationName);
	Station.nRoadID = atoi(resTable.fieldValue(2));
	Station.nRoadDirect = atoi(resTable.fieldValue(3));
	Station.nStationCard = atoi(resTable.fieldValue(4));
	Station.nStationGroup = atoi(resTable.fieldValue(5));

	//���뵽list��
	listStation.push_back(Station);

	SELECT_DATA_END();
	return true;
}

bool CSandTableDB::ReadDB(list<tagCrossing> & listCrossing)
{
	stringstream strSql;
	SELECT_DATA_BEGIN("T_Crossing", TABLE_CROSSING);

	//��������ȡ����
	tagCrossing Crossing;
	Crossing.nID = atoi(resTable.fieldValue(0));
	Crossing.strCrossingName = resTable.fieldValue(1);
	UTF8_TO_GBK(Crossing.strCrossingName);
	Crossing.nCrossingID = atoi(resTable.fieldValue(2));
	Crossing.nDirection = atoi(resTable.fieldValue(3));
	Crossing.nStraightLine = atoi(resTable.fieldValue(4));
	Crossing.nStraightDirect = atoi(resTable.fieldValue(5));
	Crossing.nDextroversionLine = atoi(resTable.fieldValue(6));
	Crossing.nDextroversionDirect = atoi(resTable.fieldValue(7));

	//���뵽list��
	listCrossing.push_back(Crossing);

	SELECT_DATA_END();
	return true;
}

bool CSandTableDB::ReadDB(list<tagCard> & listCard)
{
	stringstream strSql;
	SELECT_DATA_BEGIN("T_Card", TABLE_CARD);

	//��������ȡ����
	tagCard Card;
	Card.nID = atoi(resTable.fieldValue(0));
	Card.nCardNum = atoi(resTable.fieldValue(1));
	Card.nXpos = atoi(resTable.fieldValue(2));
	Card.nYpos = atoi(resTable.fieldValue(3));

	//���뵽list��
	listCard.push_back(Card);

	SELECT_DATA_END();
	return true;
}

bool CSandTableDB::ReadDB(int nRoadID, list<tagRoadCard>& listRoadCard)
{
	stringstream strSql;
	if (false == IsTableExist(TABLE_ROADCARD))
	{
		return false;
	}
		strSql << "SELECT * FROM T_RoadCard WHERE RoadID = '";
		strSql <<  nRoadID;
		strSql << "'";
	try
	{
		CppSQLite3Table resTable = m_DB->getTable(strSql.str().c_str());
		for (int row = 0; row < resTable.numRows(); row++)
		{
			resTable.setRow(row);

			//��������ȡ����
			tagRoadCard tRoadCard;
			tRoadCard.nID = atoi(resTable.fieldValue(0));
			tRoadCard.nRoadID = atoi(resTable.fieldValue(1));
			tRoadCard.nCardID = atoi(resTable.fieldValue(2));

			//���뵽list��
			listRoadCard.push_back(tRoadCard);

	SELECT_DATA_END();
	return true;
}

bool CSandTableDB::DeleteDB(tagRoad tRoad)
{
	CREATE_TABLE(TABLE_ROAD);
	stringstream strSQL;

	strSQL << "DELETE FROM T_RoadInfo WHERE ID = ";
	strSQL << tRoad.nID;

	//����ִ�н��
	return ExcuteSQL(strSQL.str().c_str());
}

bool CSandTableDB::DeleteDB(tagStation tStation)
{
	CREATE_TABLE(TABLE_STATION);
	stringstream strSQL;

	strSQL << "DELETE FROM T_Station WHERE ID = ";
	strSQL << tStation.nID;

	//����ִ�н��
	return ExcuteSQL(strSQL.str().c_str());
}

bool CSandTableDB::DeleteDB(tagCrossing tCrossing)
{
	CREATE_TABLE(TABLE_CROSSING);
	stringstream strSQL;

	strSQL << "DELETE FROM T_Crossing WHERE ID = ";
	strSQL << tCrossing.nID;

	//����ִ�н��
	return ExcuteSQL(strSQL.str().c_str());
}

bool CSandTableDB::DeleteDB(tagCard tCard)
{
	CREATE_TABLE(TABLE_CARD);
	stringstream strSQL;

	strSQL << "DELETE FROM T_Card WHERE ID =";
	strSQL << tCard.nID;

	//����ִ�н��
	return ExcuteSQL(strSQL.str().c_str());
}

bool CSandTableDB::DeleteDB(tagRoadCard tRoadCard)
{
	CREATE_TABLE(TABLE_ROADCARD);
	stringstream strSQL;

	strSQL << "DELETE FROM T_RoadCard WHERE RoadID =";
	strSQL << tRoadCard.nRoadID;

	//����ִ�н��
	return ExcuteSQL(strSQL.str().c_str());
}

bool CSandTableDB::ExcuteSQL(const char * pSQL)
{
	try
	{
		m_DB->execDML(pSQL);
	}
	catch (CppSQLite3Exception& e)
	{
		cout << "SQL: " << pSQL << endl;
		cerr << e.errorCode() << ":" << e.errorMessage() << endl;
		return false;
	}

	return true;
}

bool CSandTableDB::IsTableExist(const TABLE_NAME enumTableName)
{
	switch (enumTableName)
	{
	case TABLE_CARD:
	{
		return (m_DB->tableExists("T_Card") ? true : false);
	}
	break;
	case TABLE_ROAD:
	{
		return (m_DB->tableExists("T_RoadInfo") ? true : false);
	}
	break;
	case TABLE_CROSSING:
	{
		return (m_DB->tableExists("T_Crossing") ? true : false);
	}
	break;
	case TABLE_STATION:
	{
		return (m_DB->tableExists("T_Station") ? true : false);
	}
	break;
	case TABLE_ROADCARD:
		{
			return (m_DB->tableExists("T_RoadCard") ? true : false);
		}
		break;
	default:
	{
		cerr << "unknow Table Name:" << enumTableName << endl;
		return false;
	}
	break;
	}

	return false;
}

bool CSandTableDB::CreateTable(const TABLE_NAME enumTableName)
{
	//�жϱ� �Ƿ����
	if (true == IsTableExist(enumTableName))
	{
		return true;
	}

	stringstream strSQL;

	switch (enumTableName)
	{
	case TABLE_CARD:
	{
		strSQL << "CREATE TABLE `T_Card` ("\
			"`ID`	INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,"\
			"`Card`	INTEGER,"\
			"`Xpos`	INTEGER,"\
			"`Ypos`	INTEGER);";
	}
	break;
	case TABLE_ROAD:
	{
		strSQL << "CREATE TABLE `T_RoadInfo` ("\
			"`ID`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"\
			"`RoadName`	TEXT,"\
			"`Direction`	INTEGER,"\
			"`StartCard`	INTEGER,"\
			"`StartCrossingCode`	INTEGER,"\
			"`StartCrossingDirect`	INTEGER,"\
			"`EndCard`	INTEGER,"\
			"`EndCrossingCode`	INTEGER,"\
			"`EndCrossingDirect`	INTEGER);";
	}
	break;
	case TABLE_CROSSING:
	{
		strSQL << "CREATE TABLE `T_Crossing` ("\
			"`ID`	INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,"\
			"`CrossingName`	TEXT,"\
			"`CrossingID`	INTEGER,"\
			"`Direction`	INTEGER,"\
			"`StraightLine`	INTEGER,"\
			"`StraightDirect`	INTEGER,"\
			"`DextroversionLine`	INTEGER,"\
			"`DextroversionDirect`	INTEGER);";
	}
	break;
	case TABLE_STATION:
	{
		strSQL << "CREATE TABLE `T_Station` ("\
			"`ID`	INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,"\
			"`StationName`	TEXT,"\
			"`RoadID`	INTEGER,"\
			"`RoadDirect` INTEGER,"\
			"`StationCard`	INTEGER,"\
			"`StationGroup` INTEGER);";
	}
	break;
	case TABLE_ROADCARD:
	{
		strSQL << "CREATE TABLE `T_RoadCard` ("\
			"`ID`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"\
			"`RoadID`	INTEGER NOT NULL,"\
			"`CardID`	INTEGER NOT NULL);";
	}
	break;
	default:
	{
		cerr << "unknow Table Name:" << enumTableName << endl;
		return false;
	}
	break;
	}

	return ExcuteSQL(strSQL.str().c_str());
}


int GBK2UTF8(char *szGbk,char *szUtf8,int Len)
{
	// �Ƚ����ֽ�GBK��CP_ACP��ANSI��ת���ɿ��ַ�UTF-16
	// �õ�ת��������Ҫ���ڴ��ַ���
	int n = MultiByteToWideChar(CP_ACP,0,szGbk,-1,NULL,0);
	// �ַ������� sizeof(WCHAR) �õ��ֽ���
	WCHAR *str1 = new WCHAR[sizeof(WCHAR) * n];
	// ת��
	MultiByteToWideChar(CP_ACP,  // MultiByte�Ĵ���ҳCode Page
		0,            //���ӱ�־���������й�
		szGbk,        // �����GBK�ַ���
		-1,           // �����ַ������ȣ�-1��ʾ�ɺ����ڲ�����
		str1,         // ���
		n             // ������������ڴ�
		);

	// �ٽ����ַ���UTF-16��ת�����ֽڣ�UTF-8��
	n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
	if (n > Len)
	{
		delete[]str1;
		return -1;
	}
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, szUtf8, n, NULL, NULL);
	delete[]str1;
	str1 = NULL;

	return 0;
}

//UTF-8 GBK
int UTF82GBK(char *szUtf8,char *szGbk,int Len)
{
	int n = MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, NULL, 0);
	WCHAR * wszGBK = new WCHAR[sizeof(WCHAR) * n];
	memset(wszGBK, 0, sizeof(WCHAR) * n);
	MultiByteToWideChar(CP_UTF8, 0,szUtf8,-1, wszGBK, n);

	n = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	if (n > Len)
	{
		delete[]wszGBK;
		return -1;
	}

	WideCharToMultiByte(CP_ACP,0, wszGBK, -1, szGbk, n, NULL, NULL);

	delete[]wszGBK;
	wszGBK = NULL;

	return 0;
}