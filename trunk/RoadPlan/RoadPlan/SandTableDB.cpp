#include "SandTableDB.h"
#include <iostream>
#include <sstream>
#include <Windows.h>

#define SEARCH_RANGE 70

//判断数据库表是否存在  不存在则创建  创建失败则退出
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
		printf("%d:%s", e.errorCode(), e.errorMessage());
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

	//返回执行结果
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

	//返回执行结果
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

	//返回执行结果
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

	//返回执行结果
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

	//返回执行结果
	return ExcuteSQL(strSQL.str().c_str());
}

bool CSandTableDB::WriteDB(tagViolation tViolation)
{
	CREATE_TABLE(TABLE_Violation);
	stringstream strSQL;

	strSQL << "insert into T_Violation (VioType, URL) values (\"";
	GBK_TO_UTF8(tViolation.strVioType);
	strSQL << tViolation.strVioType.c_str();
	strSQL << "\",\"";
	strSQL << tViolation.strURL.c_str();
	strSQL << "\")";

	//返回执行结果
	return ExcuteSQL(strSQL.str().c_str());
}

bool CSandTableDB::WriteDB(tagCamera tCamera)
{
	CREATE_TABLE(TABLE_Camera);
	stringstream strSQL;

	strSQL << "insert into T_Camera (CameraType, CameraID) values ('";
	GBK_TO_UTF8(tCamera.strCameraType);
	strSQL << tCamera.strCameraType.c_str();
	strSQL << "',";
	strSQL << tCamera.nCameraID;
	strSQL << ")";

	//返回执行结果
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

	//将数据提取出来
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

	//插入到list中
	listRoad.push_back(RoadInfo);

	SELECT_DATA_END();

	return true;
}

bool CSandTableDB::ReadDB(list<tagStation> & listStation)
{
	stringstream strSql;
	SELECT_DATA_BEGIN("T_Station", TABLE_STATION);

	//将数据提取出来
	tagStation Station;
	Station.nID = atoi(resTable.fieldValue(0));
	Station.strStationName = resTable.fieldValue(1);
	UTF8_TO_GBK(Station.strStationName);
	Station.nRoadID = atoi(resTable.fieldValue(2));
	Station.nRoadDirect = atoi(resTable.fieldValue(3));
	Station.nStationCard = atoi(resTable.fieldValue(4));
	Station.nStationGroup = atoi(resTable.fieldValue(5));

	//插入到list中
	listStation.push_back(Station);

	SELECT_DATA_END();
	return true;
}

bool CSandTableDB::ReadDB(list<tagCrossing> & listCrossing)
{
	stringstream strSql;
	SELECT_DATA_BEGIN("T_Crossing", TABLE_CROSSING);

	//将数据提取出来
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

	//插入到list中
	listCrossing.push_back(Crossing);

	SELECT_DATA_END();
	return true;
}

bool CSandTableDB::ReadDB(list<tagCard> & listCard)
{
	stringstream strSql;
	SELECT_DATA_BEGIN("T_Card", TABLE_CARD);

	//将数据提取出来
	tagCard Card;
	Card.nID = atoi(resTable.fieldValue(0));
	Card.nCardNum = atoi(resTable.fieldValue(1));
	Card.nXpos = atoi(resTable.fieldValue(2));
	Card.nYpos = atoi(resTable.fieldValue(3));

	//插入到list中
	listCard.push_back(Card);

	SELECT_DATA_END();
	return true;
}

bool CSandTableDB::ReadDB(list<tagViolation> & listViolation)
{
	stringstream strSql;
	SELECT_DATA_BEGIN("T_Violation", TABLE_Violation);

	//将数据提取出来
	tagViolation tViolation;
	tViolation.nID = atoi(resTable.fieldValue(0));
	tViolation.strVioType = resTable.fieldValue(1);
	tViolation.strURL = resTable.fieldValue(2);
	UTF8_TO_GBK(tViolation.strVioType);

	//插入到list中
	listViolation.push_back(tViolation);

	SELECT_DATA_END();
	return true;
}

bool CSandTableDB::ReadDB(const char * pVioType, list<tagViolation> & listViolation)
{
	stringstream strSql;
	if (false == IsTableExist(TABLE_Violation))
	{
		return false;
	}

	string strVioType = pVioType;
	strSql << "SELECT * FROM T_Violation WHERE VioType = '";
	GBK_TO_UTF8(strVioType);
	strSql << strVioType;
	strSql << "'";

	try
	{
		CppSQLite3Table resTable = m_DB->getTable(strSql.str().c_str());
		if (resTable.numRows() <= 0)
		{
			return false;
		}
		for (int row = 0; row < resTable.numRows(); row++)
		{
			resTable.setRow(row);

			//将数据提取出来
			tagViolation tViolation;
			tViolation.nID = atoi(resTable.fieldValue(0));
			tViolation.strVioType = resTable.fieldValue(1);
			tViolation.strURL = resTable.fieldValue(2);
			UTF8_TO_GBK(tViolation.strVioType);

			//插入到list中
			listViolation.push_back(tViolation);
		}
	}
	catch (CppSQLite3Exception& e)
	{
		cout << "SQL: " << strSql.str().c_str() << endl;
		cerr << e.errorCode() << ":" << e.errorMessage() << endl;
		return false;
	}

	return true;
}

bool CSandTableDB::ReadDB(list<tagCamera> & listCamera)
{
	stringstream strSql;
	SELECT_DATA_BEGIN("T_Camera", TABLE_Camera);

	//将数据提取出来
	tagCamera tCamera;
	tCamera.nID = atoi(resTable.fieldValue(0));
	tCamera.strCameraType = resTable.fieldValue(1);
	UTF8_TO_GBK(tCamera.strCameraType);
	tCamera.nCameraID = atoi(resTable.fieldValue(2));

	//插入到list中
	listCamera.push_back(tCamera);

	SELECT_DATA_END();
	return true;
}

bool CSandTableDB::FindRoadFromCrossing(const int nCrossingID, const int nCrossingDirection, tagRoad & tRoad)
{
	stringstream strSql;
	if (false == IsTableExist(TABLE_CARD))
	{
		return false;
	}

	strSql << "SELECT * FROM T_RoadInfo WHERE StartCrossingCode =";
	strSql << nCrossingID;
	strSql << " AND StartCrossingDirect = ";
	strSql << nCrossingDirection;
	try
	{
		CppSQLite3Table resTable = m_DB->getTable(strSql.str().c_str());
		if (resTable.numRows() <= 0)
		{
			return false;
		}
		for (int row = 0; row < resTable.numRows(); row++)
		{
			resTable.setRow(row);

			tRoad.nID = atoi(resTable.fieldValue(0));
			tRoad.strRoadName = resTable.fieldValue(1);
			UTF8_TO_GBK(tRoad.strRoadName);
			tRoad.nDirection = atoi(resTable.fieldValue(2));
			tRoad.nStartCard = atoi(resTable.fieldValue(3));
			tRoad.nStartCrossingCode = atoi(resTable.fieldValue(4));
			tRoad.nStartCrossingDirect = atoi(resTable.fieldValue(5));
			tRoad.nEndCard = atoi(resTable.fieldValue(6));
			tRoad.nEndCrossingCode = atoi(resTable.fieldValue(7));
			tRoad.nEndCrossingDirect = atoi(resTable.fieldValue(8));

			break;
		}
	}
	catch (CppSQLite3Exception& e)
	{
		cout << "SQL: " << strSql.str().c_str() << endl;
		cerr << e.errorCode() << ":" << e.errorMessage() << endl;
		return false;
	}

	return true;
}

bool CSandTableDB::FindCrossingInfo(const tagRoad tRoad, tagCrossing & tCrossing)
{
	stringstream strSql;
	if (false == IsTableExist(TABLE_CARD))
	{
		return false;
	}

	strSql << "SELECT * FROM T_Crossing WHERE CrossingID = ";
	strSql << tRoad.nEndCrossingCode;
	strSql << " AND Direction = ";
	strSql << tRoad.nEndCrossingDirect;
	try
	{
		CppSQLite3Table resTable = m_DB->getTable(strSql.str().c_str());
		if (resTable.numRows() <= 0)
		{
			return false;
		}
		for (int row = 0; row < resTable.numRows(); row++)
		{
			resTable.setRow(row);

			tCrossing.nID = atoi(resTable.fieldValue(0));
			tCrossing.strCrossingName = resTable.fieldValue(1);
			UTF8_TO_GBK(tCrossing.strCrossingName);
			tCrossing.nCrossingID = atoi(resTable.fieldValue(2));
			tCrossing.nDirection = atoi(resTable.fieldValue(3));
			tCrossing.nStraightLine = atoi(resTable.fieldValue(4));
			tCrossing.nStraightDirect = atoi(resTable.fieldValue(5));
			tCrossing.nDextroversionLine = atoi(resTable.fieldValue(6));
			tCrossing.nDextroversionDirect = atoi(resTable.fieldValue(7));

			break;
		}
	}
	catch (CppSQLite3Exception& e)
	{
		cout << "SQL: " << strSql.str().c_str() << endl;
		cerr << e.errorCode() << ":" << e.errorMessage() << endl;
		return false;
	}

	return true;
}

bool CSandTableDB::FindCardInfo(const int nCardID, tagCard & tCard)
{
	stringstream strSql;
	if (false == IsTableExist(TABLE_CARD))
	{
		return false;
	}

	strSql << "SELECT * FROM T_Card WHERE ID = ";
	strSql << nCardID;
	try
	{
		CppSQLite3Table resTable = m_DB->getTable(strSql.str().c_str());
		if (resTable.numRows() <= 0)
		{
			return false;
		}
		for (int row = 0; row < resTable.numRows(); row++)
		{
			resTable.setRow(row);

			tCard.nID = atoi(resTable.fieldValue(0));
			tCard.nCardNum = atoi(resTable.fieldValue(1));
			tCard.nXpos = atoi(resTable.fieldValue(2));
			tCard.nYpos = atoi(resTable.fieldValue(3));

			break;
		}
	}
	catch (CppSQLite3Exception& e)
	{
		cout << "SQL: " << strSql.str().c_str() << endl;
		cerr << e.errorCode() << ":" << e.errorMessage() << endl;
		return false;
	}

	return true;
}

bool CSandTableDB::FindRoadFromCard(const int nCardID, tagRoad & tRoad)
{
	stringstream strSql;
	if (false == IsTableExist(TABLE_ROADCARD))
	{
		return false;
	}

	strSql << "SELECT * FROM T_RoadInfo WHERE ID = (SELECT RoadID FROM T_RoadCard Where CardID = ";
	strSql << nCardID;
	strSql << ")";
	try
	{
		CppSQLite3Table resTable = m_DB->getTable(strSql.str().c_str());
		if (resTable.numRows() <= 0)
		{
			return false;
		}
		for (int row = 0; row < resTable.numRows(); row++)
		{
			resTable.setRow(row);
			
			tRoad.nID = atoi(resTable.fieldValue(0));
			tRoad.strRoadName = resTable.fieldValue(1);
			UTF8_TO_GBK(tRoad.strRoadName);
			tRoad.nDirection = atoi(resTable.fieldValue(2));
			tRoad.nStartCard = atoi(resTable.fieldValue(3));
			tRoad.nStartCrossingCode = atoi(resTable.fieldValue(4));
			tRoad.nStartCrossingDirect = atoi(resTable.fieldValue(5));
			tRoad.nEndCard = atoi(resTable.fieldValue(6));
			tRoad.nEndCrossingCode = atoi(resTable.fieldValue(7));
			tRoad.nEndCrossingDirect = atoi(resTable.fieldValue(8));
			break;
		}
	}
	catch (CppSQLite3Exception& e)
	{
		cout << "SQL: " << strSql.str().c_str() << endl;
		cerr << e.errorCode() << ":" << e.errorMessage() << endl;
		return false;
	}

	return true;
}

bool CSandTableDB::FindCardFromPos(const int nXpos, const int nYpos, list<tagCard> & lstCard)
{
	stringstream strSql;
	if (false == IsTableExist(TABLE_CARD))
	{
		return false;
	}

	strSql << "SELECT * FROM T_Card WHERE Xpos > ";
	strSql << nXpos - SEARCH_RANGE;
	strSql << " AND Xpos < ";
	strSql << nXpos + SEARCH_RANGE;
	strSql << " AND Ypos > ";
	strSql << nYpos - SEARCH_RANGE;
	strSql << " AND Ypos < ";
	strSql << nYpos + SEARCH_RANGE;
	
	printf("SQL: %s\n", strSql.str().c_str());

	try
	{
		CppSQLite3Table resTable = m_DB->getTable(strSql.str().c_str());
		if (resTable.numRows() <= 0)
		{
			return false;
		}
		for (int row = 0; row < resTable.numRows(); row++)
		{
			resTable.setRow(row);

			tagCard tCard;

			tCard.nID = atoi(resTable.fieldValue(0));
			tCard.nCardNum = atoi(resTable.fieldValue(1));
			tCard.nXpos = atoi(resTable.fieldValue(2));
			tCard.nYpos = atoi(resTable.fieldValue(3));

			lstCard.push_back(tCard);
		}
	}
	catch (CppSQLite3Exception& e)
	{
		cout << "SQL: " << strSql.str().c_str() << endl;
		cerr << e.errorCode() << ":" << e.errorMessage() << endl;
		return false;
	}

	return true;
}

bool CSandTableDB::DeleteDB(tagViolation tViolation)
{
	CREATE_TABLE(TABLE_Violation);
	stringstream strSQL;

	strSQL << "DELETE FROM T_Violation WHERE ID = ";
	strSQL << tViolation.nID;

	//返回执行结果
	return ExcuteSQL(strSQL.str().c_str());
}

bool CSandTableDB::DeleteDB(tagCamera tCamera)
{

	CREATE_TABLE(TABLE_Camera);
	stringstream strSQL;

	strSQL << "DELETE FROM T_Camera";

	//返回执行结果
	return ExcuteSQL(strSQL.str().c_str());
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

			//将数据提取出来
			tagRoadCard tRoadCard;
			tRoadCard.nID = atoi(resTable.fieldValue(0));
			tRoadCard.nRoadID = atoi(resTable.fieldValue(1));
			tRoadCard.nCardID = atoi(resTable.fieldValue(2));

			//插入到list中
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

	//返回执行结果
	return ExcuteSQL(strSQL.str().c_str());
}

bool CSandTableDB::DeleteDB(tagStation tStation)
{
	CREATE_TABLE(TABLE_STATION);
	stringstream strSQL;

	strSQL << "DELETE FROM T_Station WHERE ID = ";
	strSQL << tStation.nID;

	//返回执行结果
	return ExcuteSQL(strSQL.str().c_str());
}

bool CSandTableDB::DeleteDB(tagCrossing tCrossing)
{
	CREATE_TABLE(TABLE_CROSSING);
	stringstream strSQL;

	strSQL << "DELETE FROM T_Crossing WHERE ID = ";
	strSQL << tCrossing.nID;

	//返回执行结果
	return ExcuteSQL(strSQL.str().c_str());
}

bool CSandTableDB::DeleteDB(tagCard tCard)
{
	CREATE_TABLE(TABLE_CARD);
	stringstream strSQL;

	strSQL << "DELETE FROM T_Card WHERE ID =";
	strSQL << tCard.nID;

	//返回执行结果
	return ExcuteSQL(strSQL.str().c_str());
}

bool CSandTableDB::DeleteDB(tagRoadCard tRoadCard)
{
	CREATE_TABLE(TABLE_ROADCARD);
	stringstream strSQL;

	strSQL << "DELETE FROM T_RoadCard WHERE RoadID =";
	strSQL << tRoadCard.nRoadID;

	//返回执行结果
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
	case TABLE_Violation:
	{
		return (m_DB->tableExists("T_Violation") ? true : false);
	}
	break;
	case TABLE_Camera:
	{
		return (m_DB->tableExists("T_Camera") ? true : false);
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
	//判断表 是否存在
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
	case TABLE_Violation:
	{
		strSQL << "CREATE TABLE `T_Violation` ("\
			"`ID`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"\
			"`VioType`	TEXT NOT NULL,"\
			"`URL`	TEXT NOT NULL);";
	}
	break;
	case TABLE_Camera:
	{
		strSQL << "CREATE TABLE `T_Camera` ("\
			"`ID`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"\
			"`CameraType`	TEXT NOT NULL,"\
			"`CameraID`	INTEGER NOT NULL);";
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
	// 先将多字节GBK（CP_ACP或ANSI）转换成宽字符UTF-16
	// 得到转换后，所需要的内存字符数
	int n = MultiByteToWideChar(CP_ACP,0,szGbk,-1,NULL,0);
	// 字符数乘以 sizeof(WCHAR) 得到字节数
	WCHAR *str1 = new WCHAR[sizeof(WCHAR) * n];
	// 转换
	MultiByteToWideChar(CP_ACP,  // MultiByte的代码页Code Page
		0,            //附加标志，与音标有关
		szGbk,        // 输入的GBK字符串
		-1,           // 输入字符串长度，-1表示由函数内部计算
		str1,         // 输出
		n             // 输出所需分配的内存
		);

	// 再将宽字符（UTF-16）转换多字节（UTF-8）
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